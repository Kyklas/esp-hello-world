# allows to automate help generation


# Global var for extended help info on new line
HLP_NL=\n\t\t\t

#
# $1 - target
# $2 - description
#
define help-add

HLP_$(1)_STRING:=$2
HLP_TARGET_LIST+=$1

endef

HLP_TARGET_LIST?=

#
# $1 - target
# $2 - description
#
define help-var-add

HLP_V$(1)_STRING:=$2
HLP_VAR_LIST+=$1

endef

HLP_VAR_LIST?=

HLP_FULL?=

$(eval $(call help-add,help-print,Print Project Help))
help-print:
	echo ""
	echo The defined target help are :
	
	$(foreach HLP_VAR, $(HLP_VAR_LIST), \
	$(if $(filter NULL,$(HLP_VAR)),echo "";, \
	printf "`tput setaf 5;tput bold`%20s`tput sgr0` : " $(HLP_VAR); printf "`tput setaf 2;tput bold`$(HLP_V$(HLP_VAR)_STRING)`tput sgr0`\n"; ))
	echo ""
	$(foreach HLP_TARGET, $(HLP_TARGET_LIST), \
	$(if $(filter NULL,$(HLP_TARGET)),echo "";, \
	printf "`tput setaf 3;tput bold`%20s`tput sgr0` : " $(HLP_TARGET); printf "`tput setaf 6;tput bold`$(HLP_$(HLP_TARGET)_STRING)`tput sgr0`\n"; ))
	echo ""

$(eval $(call help-add,local-help,Print Project Help and END \
$(HLP_NL) - If HLP_FULL variable is set; full help is shown ))
local-help: help-print
	$(if $(HLP_FULL),,$(error End XDC Help ))

$(eval $(call help-add,help,Print Project Help and IDF help  \
$(HLP_NL) - If HLP_FULL variable is set; full help is shown ))
help: local-help

define help-space
$(eval $(call help-add,NULL,NULL))
endef

$(eval $(call help-space))
