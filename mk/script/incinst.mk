
.PHONY : all include_install include_install_dist

all: include_install_dist

include mk/image_lib.mk
INCLUDE_INSTALL := include_install

.SECONDEXPANSION:
include $(MKGEN_DIR)/include.mk
include $(MKGEN_DIR)/include_install.rule.mk


include $(__include_incinst)

cp_T_if_supported := $(shell $(CP) --version 2>&1 | grep -l GNU >/dev/null && echo -T)

############################
include_install_prerequisites = $(src_file) $(common_prereqs)
$(INCUDE_INSTALL_DIR)/%/. : | $(INCUDE_INSTALL_DIR)/.
	@mkdir -p $(@D)
	@touch $@

$(INCUDE_INSTALL_DIR)/% : | $(INCUDE_INSTALL_DIR)/.
	$(CP) -r $(cp_T_if_supported) $(src_file) $@

$(INCUDE_INSTALL_DIR)/. :	
	@$(MKDIR) $(@D)

#include_install_dist_prerequisites = $(include_install_files)
include_install_dist: $(INCLUDE_INSTALL) $(include_install_files)

#################################