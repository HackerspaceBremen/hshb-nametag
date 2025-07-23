#!/bin/bash -e
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
	echo " bash ${SCRIPT_EXECUTABLE}"
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

# LOGO see https://textkool.com/en/ascii-art-generator?hl=default&vl=default&font=Big&text=hexbuilder

log "  _               _           _ _     _           "
log " | |             | |         (_) |   | |          "
log " | |__   _____  _| |__  _   _ _| | __| | ___ _ __ "
log " | '_ \\ / _ \\ \/ / '_ \\| | | | | |/ _\` |/ _ \\ '__|"
log " | | | |  __/>  <| |_) | |_| | | | (_| |  __/ |   "
log " |_| |_|\\___/_/\\_\\_.__/ \\__,_|_|_|\\__,_|\\___|_|   "
log "                                                  "

SCRIPT_USER="undefined"
SCRIPT_DEFAULT_USER=""
SCRIPT_NAME="HEXBUILDER"
SCRIPT_PRESS_ANY_KEY="Press any key to continue or CTRL-C to abort"
SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

log green "LAUNCHING FROM: ${SCRIPT_DIR} ..."

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
log grey "Following script will create"
log grey "builds for all boards in one go."
log
log hint "${SCRIPT_PRESS_ANY_KEY}"
read -r -n 1

# CHECK IF PLATFORM IO IS INSTALLED
log info "INSTALLER: CHECKING INSTALLED pio VERSION"
SCRIPT_FALSE="FALSE"
SCRIPT_PIO_ENV="${HOME}/.platformio/penv/"
SCRIPT_PIO_BINARY="${HOME}/.platformio/penv/bin/pio"
SCRIPT_HAS_PIO=$(command -v "${SCRIPT_PIO_BINARY}" 2>/dev/null || echo $SCRIPT_FALSE)
if [[ $SCRIPT_HAS_PIO == "${SCRIPT_FALSE}" ]]; then
	log
	log red "PIO BINARY '${SCRIPT_PIO_BINARY}' NECESSARY FOR INSTALLATION."
	log white "DID NOT FIND PATH TO BINARY OF PLATFORM-IO '${SCRIPT_PIO_BINARY}'"
	log white "PLEASE PROVIDE A PATH TO '${SCRIPT_PIO_BINARY}' TO RUN THIS."
	log warn "ERROR"
	exit 1
else
	log green "PIO BINARY '${SCRIPT_PIO_BINARY}' IS ALREADY PRESENT."
	SCRIPT_PIO_PATH=$(which "${SCRIPT_PIO_BINARY}")
	log white "${SCRIPT_PIO_PATH}"
	log white "$($SCRIPT_PIO_BINARY --version)"
	log ok "OK"
fi
log hint "${SCRIPT_PRESS_ANY_KEY}"
read -r -n 1

# ASK BACK FOR START BUILDING
SCRIPT_SHOULD_CONTINUE=0
log attn " WANT TO BUILD ALL FIRMWARES NOW? "
while true; do
	SCRIPT_ASK=$(log yellow " Y)es / N)o : ")
	read -r -p "${SCRIPT_ASK}" INPUT
	case $INPUT in
	[Yy]*)
		SCRIPT_SHOULD_CONTINUE=1
		break
		;;
	[Nn]*)
		SCRIPT_SHOULD_CONTINUE=0
		log warn "ABORTED"
		break
		;;
	*)
		SCRIPT_SHOULD_CONTINUE=0
		log warn "ABORTED"
		break
		;;
	esac
done

if [[ $SCRIPT_SHOULD_CONTINUE -gt 0 ]]; then
	SCRIPT_PIO_PATH_TO_BUILD=".pio/build/ATmega32/firmware.hex"
	NUMBER_OF_BOARDS=2 # ENTER NUMBER OF BOARDS HERE
	source "${SCRIPT_PIO_ENV}/bin/activate"
	log
	log cyan-b "BUILDING FOR ${NUMBER_OF_BOARDS} BOARDS IN PROGRESS... (CTRL-C TO ABORT)"
	log
	for ((i = 1; i <= NUMBER_OF_BOARDS; i++)); do
		log cyan "BOARD ${i} BUILDING..."
		# CLEANUP OF BUILD FOLDER
		$SCRIPT_PIO_BINARY run -t clean 2>&1 | grep -E "Removing|cleaning"
		# BUILD NEW BINARY WITH SPECIFIC TO BOARD WITH ID
		BOARD_ID=$i $SCRIPT_PIO_BINARY run 2>&1 | grep -E "PLATFORM|HARDWARE|Archiving|Linking|Checking|RAM:|Flash:|Building"
		# COPY BINARY TO TARGET FILENAME
		cp "${SCRIPT_PIO_PATH_TO_BUILD}" "./hshb-nametag-${i}.hex"
		log ok "OK"
	done
	log ok "FINISHED"
fi

log info "*** ${SCRIPT_NAME} COMPLETE - VERSION ${SCRIPT_VERSION} BUILD ${SCRIPT_BUILD} ***"
log info "*** GOOD BYE! ***"
log
