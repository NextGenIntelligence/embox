/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    21.01.2014
 */

#include <errno.h>
#include <endian.h>
#include <string.h>
#include <drivers/usb/usb.h>
#include "usb_mass_storage.h"

#include "scsi.h"

static void usb_scsi_notify(struct usb_request *req, void *arg) {
	struct usb_mscsw *csw = arg;
	struct scsi_dev *sdev = &(usb2massdata(req->endp->dev)->scsi_dev);

	if (csw->csw_status != 0) {
		assert(csw->csw_status == 1);
		scsi_request_done(sdev, -1);
		return;
	}

	scsi_request_done(sdev, 0);
}

int scsi_cmd(struct scsi_dev *sdev, void *cmd, size_t cmd_len, void *data, size_t data_len) {
	struct usb_mass *mass = member_cast_out(sdev, struct usb_mass, scsi_dev);

	return usb_ms_transfer(mass->usb_dev, cmd, cmd_len, USB_DIRECTION_IN, data, data_len,
			usb_scsi_notify);
}

#define SCSI_CMD_LEN 16
static int scsi_do_cmd(struct scsi_dev *dev, struct scsi_cmd *cmd) {
	uint8_t scmd[SCSI_CMD_LEN];

	assert(cmd->scmd_len <= SCSI_CMD_LEN);
	memset(scmd + 1, 0, cmd->scmd_len - 1);
	scmd[0] = cmd->scmd_opcode;

	if (cmd->scmd_fixup) {
		cmd->scmd_fixup(scmd, cmd);
	}

	return scsi_cmd(dev, scmd, cmd->scmd_len, cmd->scmd_obuf,
			cmd->scmd_olen);
}

static void scsi_fixup_inquiry(void *buf, struct scsi_cmd *cmd) {
	struct scsi_cmd_inquiry *cmd_inquiry = buf;
	cmd_inquiry->sinq_alloc_length = htobe16(cmd->scmd_olen);
}

static void scsi_fixup_read_sense(void *buf, struct scsi_cmd *cmd) {
	struct scsi_cmd_sense *cmd_sense = buf;
	cmd_sense->ssns_alloc_length = cmd->scmd_olen;
}

static const struct scsi_cmd scsi_cmd_template_inquiry = {
	.scmd_opcode = SCSI_CMD_OPCODE_INQUIRY,
	.scmd_len = sizeof(struct scsi_cmd_inquiry),
	.scmd_fixup = scsi_fixup_inquiry,
};

static const struct scsi_cmd scsi_cmd_template_cap10 = {
	.scmd_opcode = SCSI_CMD_OPCODE_READ_CAP10,
	.scmd_len = sizeof(struct scsi_cmd_cap10),
};

static const struct scsi_cmd scsi_cmd_template_sense = {
	.scmd_opcode = SCSI_CMD_OPCODE_REQ_SENSE,
	.scmd_len = sizeof(struct scsi_cmd_sense),
	.scmd_fixup = scsi_fixup_read_sense,
};

int scsi_dev_init(struct scsi_dev *dev) {
	return 0;
}

static inline void scsi_state_transit(struct scsi_dev *dev,
		const struct scsi_dev_state *to) {
	const struct scsi_dev_state *from = dev->state;

	if (from && from->sds_leave)
		from->sds_leave(dev);

	if (to->sds_enter)
		to->sds_enter(dev);

	dev->state = to;
}

static const struct scsi_dev_state scsi_state_inquiry;
static const struct scsi_dev_state scsi_state_capacity;
static const struct scsi_dev_state scsi_state_sense;
static const struct scsi_dev_state scsi_state_sense;

static void scsi_final_enter(struct scsi_dev *dev) {

}

static const struct scsi_dev_state scsi_state_final = {
	.sds_enter = scsi_final_enter,
};

static void scsi_dev_recover(struct scsi_dev *dev);

static void scsi_inquiry_enter(struct scsi_dev *dev) {
	struct scsi_cmd cmd = scsi_cmd_template_inquiry;
	cmd.scmd_obuf = dev->scsi_data_scratchpad;
	cmd.scmd_olen = sizeof(struct scsi_data_inquiry);

	scsi_do_cmd(dev, &cmd);
}

static void scsi_inquiry_input(struct scsi_dev *dev, int res) {
	struct scsi_data_inquiry *data;

	assert(res == 0);

	data = (struct scsi_data_inquiry *) dev->scsi_data_scratchpad;
	if ((data->dinq_devtype & SCSI_INQIRY_DEVTYPE_MASK)
			!= SCSI_INQIRY_DEVTYPE_BLK) {
		return;
	}


	scsi_state_transit(dev, &scsi_state_capacity);
}

static const struct scsi_dev_state scsi_state_inquiry = {
	.sds_enter = scsi_inquiry_enter,
	.sds_input = scsi_inquiry_input,
};

static void scsi_capacity_enter(struct scsi_dev *dev) {
	struct scsi_cmd cmd = scsi_cmd_template_cap10;
	cmd.scmd_obuf = dev->scsi_data_scratchpad;
	cmd.scmd_olen = sizeof(struct scsi_data_cap10);

	scsi_do_cmd(dev, &cmd);
}

static void scsi_capacity_input(struct scsi_dev *dev, int res) {

	if (res < 0) {
		scsi_dev_recover(dev);
		return;
	}

	scsi_state_transit(dev, &scsi_state_final);
}

static const struct scsi_dev_state scsi_state_capacity = {
	.sds_enter = scsi_capacity_enter,
	.sds_input = scsi_capacity_input,
};

static void scsi_sense_enter(struct scsi_dev *dev) {
	struct scsi_cmd cmd = scsi_cmd_template_sense;
	cmd.scmd_obuf = dev->scsi_data_scratchpad;
	cmd.scmd_olen = sizeof(struct scsi_data_sense);

	scsi_do_cmd(dev, &cmd);
}

static void scsi_sense_input(struct scsi_dev *dev, int res) {
	struct scsi_data_sense *data;
	uint8_t acode;

	assert(res == 0);

	data = (struct scsi_data_sense *) dev->scsi_data_scratchpad;
	acode = data->dsns_additional_code;
	assert(acode == 0x28 || acode == 0x29, "Don't know how to recover "
			"unknown error %x", acode);

	/* 0x28 and 0x29 are just required attention, seems that can go on */

	scsi_state_transit(dev, dev->holded_state);
	dev->holded_state = NULL;
}

static const struct scsi_dev_state scsi_state_sense = {
	.sds_enter = scsi_sense_enter,
	.sds_input = scsi_sense_input,
};

static void scsi_dev_recover(struct scsi_dev *dev) {

	assert(dev->holded_state == NULL, "Can't recover recovering procedure");

	dev->holded_state = dev->state;

	scsi_state_transit(dev, &scsi_state_sense);
}

void scsi_dev_attached(struct scsi_dev *dev) {

	dev->state = NULL;
	scsi_state_transit(dev, &scsi_state_inquiry);
}

void scsi_request_done(struct scsi_dev *dev, int res) {

	if (dev->state && dev->state->sds_input) {
		dev->state->sds_input(dev, res);
	}
}
