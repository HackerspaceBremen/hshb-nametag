#!/bin/bash
# SCRIPT TO UPDATE AND BACKUP FIRMWARE
#
# LAST UPDATED: JUL 2025
#      RELEASE: 1.0
#        BUILD: 1

SCRIPT_VERSION="1.0"
SCRIPT_BUILD="1"
SCRIPT_EXECUTABLE="${0}"

# BASIC CHECK FOR REALLY RUNNING IN BASH
if [ -z "$BASH_VERSION" ]; then
	echo "ERROR: THIS SCRIPT NEEDS TO RUN IN A BASH SHELL"
	echo "PLEASE TRY EXECUTING THE FOLLOWING TO SUCCEED:"
	echo ""
	echo " bash ./hshb_tag_update.sh"
	echo ""
	exit 1
fi

# MAKE SCRIPT FAIL IF WE ENCOUNTER A NON-ZERO RETURN CODE IN
# EITHER DIRECT COMMAND EXECUTION OR NON-ZERO RETURNS WITHIN
# PIPED COMMANDS
set -eou pipefail

# CONVENIENCE FUNCTION TO PROVIDE COLORIZED echo-OUTPUT
log() {
	if [[ $# == 0 ]]; then
		echo -e "" # NO ARGUMENTS GIVE EMPTY echoe AS NEW LINE
	else
		COLR="" # CONTAINTS THE ESCAPE SEQUENCE FOR SETTING TERMINAL COLOR MODE
		LFD=""  # ADDS OPTIONAL LINEFEED AT BEFORE AND AFTER OUTPUT CONTENT
		SPC=""  # ADDITIONAL SPACES ADDED BEFORE AND AFTER
		case $1 in
		"red") COLR="\033[31m" ;;
		"red-b") COLR="\033[1;5;31m" ;; # -b APPENDED WILL MAKE COLOR BLINK & BOLD
		"green") COLR="\033[32m" ;;
		"green-b") COLR="\033[1;5;32m" ;;
		"yellow") COLR="\033[33m" ;;
		"yellow-b") COLR="\033[1;5;33m" ;;
		"blue") COLR="\033[34m" ;;
		"blue-b") COLR="\033[1;5;34m" ;;
		"purple") COLR="\033[35m" ;;
		"purple-b") COLR="\033[1;5;35m" ;;
		"cyan") COLR="\033[36m" ;;
		"cyan-b") COLR="\033[1;5;36m" ;;
		"white") COLR="\033[37m" ;;
		"white-b") COLR="\033[1;5;37m" ;;
		"grey") COLR="\033[2;37m" ;;
		"grey-b") COLR="\033[2;5;37m" ;;
		"black") COLR="\033[30m" ;;
		"black-b") COLR="\033[1;5;30m" ;;
		"ok")
			COLR="\033[1;42;30m"
			LFD="\n"
			SPC="   "
			;; # ok, attn, warn, info, hint & fatal USE INVERSE COLORED BACKGROUND
		"ok-b")
			COLR="\033[1;5;42;30m"
			LFD="\n"
			SPC="   "
			;;
		"hint")
			COLR="\033[1;47;30m"
			LFD="\n"
			SPC="   "
			;;
		"hint-b")
			COLR="\033[1;5;47;30m"
			LFD="\n"
			SPC="   "
			;;
		"attn")
			COLR="\033[1;43;30m"
			LFD="\n"
			SPC="   "
			;;
		"attn-b")
			COLR="\033[1;5;43;30m"
			LFD="\n"
			SPC="   "
			;;
		"warn")
			COLR="\033[1;41;30m"
			LFD="\n"
			SPC="   "
			;;
		"warn-c")
			COLR="\033[1;41;30m"
			LFD=""
			SPC="   "
			;;
		"warn-b")
			COLR="\033[1;5;41;37m"
			LFD="\n"
			SPC="   "
			;;
		"info")
			COLR="\033[1;44;37m"
			LFD="\n"
			SPC="   "
			;;
		"info-b")
			COLR="\033[1;5;44;37m"
			LFD="\n"
			SPC="   "
			;;
		"fatal")
			COLR="\033[1;45;30m"
			LFD="\n"
			SPC="   "
			;;
		"fatal-b")
			COLR="\033[1;5;45;30m"
			LFD="\n"
			SPC="   "
			;;
		*)
			echo -e "$1"
			return
			;; # IF ONLY CONTENT AS ARGUMENT AND NO COLOR INFO
		esac
		RST="\033[0m"                                     # RESETS ALL COLOR MODES ACTIVATED
		echo -e "${LFD}${COLR}${SPC}$2${SPC}${RST}${LFD}" # ECHOS SECOND ARGUMENT PASSED IN WITH COLOR FROM ARG 1
	fi
}

# HELPER FUNCTION TO DETECT CTRL-C
script_ctrlc() {
	log ""
	log warn "ABORTED"
	log info "*** GOOD BYE! (CTRL-C) ***"
	exit 1 # THIS IS IMPORTANT!!!
}

# HELPER TO EXIT THE NICE WAY WITH AN ERROR
script_error() {
	log ""
	log warn "ERROR"
	log "SCRIPT FAILED TO EXECUTE. SEE ERROR ABOVE."
	log info "*** GOOD BYE! (ERROR) ***"
}

# HELPER TO ABORT THE SCRIPT PROGRAMMATICALLY
script_abort() {
	log ""
	log warn "ABORTED"
	log info "*** GOOD BYE! (EXITING) ***"
	exit 1
}

# HELPER TO ASK FOR CONTINUATION
script_continue() {
	while true; do
		read -r -p "? " yn
		case $yn in
		[Yy]*) return 0 ;; # RETURN 0 if YES IS CHOSEN
		[Nn]*) return 1 ;; # RETURN 1 if NO IS CHOSEN
		*) ;;              # DO NOTHING ON OTHER INPUTS
		esac
	done
}

# INTERCEPT UPCOMING ERRORS (EXIT'S WITH NON-ZERO) & INTERRUPTIONS (CTRL-C)
trap "script_error" ERR
trap "script_ctrlc" INT

# LOGO see https://textkool.com/en/ascii-art-generator?hl=controlled%20smushing&vl=default&font=Big&text=FieldServices

log "  _               __ _           _          "
log " | |             / _| |         | |         "
log " | |_ __ _  __ _| |_| | __ _ ___| |__  _ __ "
log " | __/ _\` |/ _\` |  _| |/ _\` / __| '_ \\| '__|"
log " | || (_| | (_| | | | | (_| \\__ \\ | | | |   "
log "  \\__\\__,_|\\__, |_| |_|\\__,_|___/_| |_|_|   "
log "            __/ |                           "
log "           |___/                            "

SCRIPT_USER="undefined"
SCRIPT_DEFAULT_USER=""
SCRIPT_NAME="TAGFLASHR"
SCRIPT_PRESS_ANY_KEY="Press any key to continue or CTRL-C to abort"
SCRIPT_SERIAL_DEVICE="/dev/tty.usbserial-314410"

# DISALLOW THIS TO BE RUN AS ROOT USER
if [[ $UID -eq 0 ]]; then
	log warn-b "THIS SCRIPT SHOULD NEVER BE RUN WITH ROOT USER (IT WILL REQUEST MORE RIGHTS IF NEEDED) ..."
	echo -n "ENTER DIFFERENT MACHINE USER (DEFAULT '${SCRIPT_DEFAULT_USER}'): "
	read -r SCRIPT_USER
	if [ -z "${SCRIPT_USER}" ]; then
		SCRIPT_USER=$SCRIPT_DEFAULT_USER
		log green "TRYING TO RE-EXECUTE WITH USER: ${SCRIPT_USER} ..."
	else
		log green "TRYING TO RE-EXECUTE WITH USER: ${SCRIPT_USER} ..."
	fi
	sudo -u "${SCRIPT_USER} ${SCRIPT_EXECUTABLE}" || log warn-b "FAILED TO EXECUTE WITH USER '${SCRIPT_USER}' ..."
fi

if [[ $UID -eq 0 ]]; then
	log warn "ROOT USER STILL ACTIVE (ABORTING) ..."
	script_abort
fi

log info "*** WELCOME TO ${SCRIPT_NAME} - VERSION ${SCRIPT_VERSION} BUILD ${SCRIPT_BUILD} ***"
log
log grey "Following script will help backup"
log grey "and flash your HSHB LED Tag"
log
log hint "${SCRIPT_PRESS_ANY_KEY}"
read -r -n 1

# ASK FOR USB-PORT/DEVICE
if [[ $# -gt 0 ]]; then
	SCRIPT_SERIAL_DEVICE=$1
	log green "USING SERIAL DEVICE FROM ARGS: ${SCRIPT_SERIAL_DEVICE}"
else
	echo -n "ENTER SERIAL DEVICE PATH (DEFAULT '${SCRIPT_SERIAL_DEVICE}'): "
	read -r SCRIPT_SERIAL_DEVICE_NEW
	if [ -z "${SCRIPT_SERIAL_DEVICE_NEW}" ]; then
		log green "USING DEFAULT DEVICE: ${SCRIPT_SERIAL_DEVICE}"
	else
		SCRIPT_SERIAL_DEVICE=$SCRIPT_SERIAL_DEVICE_NEW
		log green "USING ENTERED DEVICE: ${SCRIPT_SERIAL_DEVICE_NEW}"
	fi
fi

# ASK BACK FOR BACKUP
SCRIPT_DO_BACKUP=0
log attn-b " WANT TO PULL A BACKUP FROM DEVICE NOW? "
while true; do
	SCRIPT_ASK=$(log yellow " Y)es / N)o : ")
	read -r -p "${SCRIPT_ASK}" INPUT
	case $INPUT in
	[Yy]*)
		SCRIPT_DO_BACKUP=1
		break
		;;
	[Nn]*)
		SCRIPT_DO_BACKUP=0
		log warn "ABORTED"
		break
		;;
	*)
		SCRIPT_DO_BACKUP=0
		log warn "ABORTED"
		break
		;;
	esac
done

if [[ $SCRIPT_DO_BACKUP -gt 0 ]]; then
	SCRIPT_FILENAME_BACKUP="firmware_backup.hex"
	log
	log warn " PLEASE PROVIDE FILENAME FOR BACKUP "
	echo -n "ENTER PATH TO BACKUP FILE (DEFAULT '${SCRIPT_FILENAME_BACKUP}'): "
	read -r SCRIPT_FILENAME_BACKUP_NEW
	if [ -z "${SCRIPT_FILENAME_BACKUP_NEW}" ]; then
		log green "USING DEFAULT BACKUP FILE: ${SCRIPT_FILENAME_BACKUP}"
	else
		SCRIPT_FILENAME_BACKUP=$SCRIPT_FILENAME_BACKUP_NEW
		log green "USING ENTERED BACKUP FILE: ${SCRIPT_FILENAME_BACKUP_NEW}"
	fi

	log
	log cyan "BACKUP IN PROGRESS..."
	log
	avrdude -p atmega32 -C avrdude.conf -c arduino -b 38400 -D -P "${SCRIPT_SERIAL_DEVICE}" -U "flash:r:${SCRIPT_FILENAME_BACKUP}:i" 2>&1 | grep -E "Reading|Writing"
	log
	log ok "OK"
	log
fi

# ASK BACK FOR UPDATE
SCRIPT_DO_UPDATE=0
log attn-b " WANT TO PUSH AN UPDATE TO DEVICE NOW? "
while true; do
	SCRIPT_ASK=$(log yellow " Y)es / N)o : ")
	read -r -p "${SCRIPT_ASK}" INPUT
	case $INPUT in
	[Yy]*)
		SCRIPT_DO_UPDATE=1
		break
		;;
	[Nn]*)
		SCRIPT_DO_UPDATE=0
		log warn "ABORTED"
		break
		;;
	*)
		SCRIPT_DO_UPDATE=0
		log warn "ABORTED"
		break
		;;
	esac
done

if [[ $SCRIPT_DO_UPDATE -gt 0 ]]; then
	SCRIPT_FILENAME_UPDATE="firmware_update.hex"
	log
	log warn " PLEASE PROVIDE FILENAME FOR UPDATE "
	echo -n "ENTER PATH TO UPDATE FILE (DEFAULT '${SCRIPT_FILENAME_UPDATE}'): "
	read -r SCRIPT_FILENAME_UPDATE_NEW
	if [ -z "${SCRIPT_FILENAME_UPDATE_NEW}" ]; then
		log green "USING DEFAULT UPDATE FILE: ${SCRIPT_FILENAME_UPDATE}"
	else
		SCRIPT_FILENAME_UPDATE=$SCRIPT_FILENAME_UPDATE_NEW
		log green "USING ENTERED UPDATE FILE: ${SCRIPT_FILENAME_UPDATE_NEW}"
	fi
	log
	log cyan "UPDATE IN PROGRESS..."
	log
	avrdude -p atmega32 -C avrdude.conf -c arduino -b 38400 -D -P "${SCRIPT_SERIAL_DEVICE}" -U "flash:w:${SCRIPT_FILENAME_UPDATE}:i" 2>&1 | grep -E "Reading|Writing"
	log
	log ok "OK"
	log
fi

log info "*** ${SCRIPT_NAME} COMPLETE - VERSION ${SCRIPT_VERSION} BUILD ${SCRIPT_BUILD} ***"
log info "*** GOOD BYE! ***"
log
