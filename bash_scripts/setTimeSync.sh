#!/bin/bash

# script MD Harrington 2 May 2020
# Time 00:05hrs 

# Variable name reminders 

#
#   $0 - The name of the Bash script.
#   $1 - $9 - The first 9 arguments to the Bash script. (As mentioned above.)
#   $# - How many arguments were passed to the Bash script.
#   $@ - All the arguments supplied to the Bash script.
#   $? - The exit status of the most recently run process.
#   $$ - The process ID of the current script.
#   $USER - The username of the user running the script.
#   $HOSTNAME - The hostname of the machine the script is running on.
#   $SECONDS - The number of seconds since the script was started.
#   $RANDOM - Returns a different random number each time is it referred to.
#   $LINENO - Returns the current line number in the Bash script.




STD='\033[0;0;39m'		  #reset 
IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
IBlue='\033[0;94m'        # Blue
IPurple='\033[0;95m'      # Purple
ICyan='\033[0;96m'        # Cyan
IWhite='\033[0;97m'       # White

#global variable 
CURRENTDATEONLY=`date +"%A, %b %d, %Y %H:%M:%S"`

#important must have this otherwise no go 

FILE=/etc/ntp.conf
FILEBCK=/etc/ntp.conf.back

timedatectl | grep Time




show_menus() {
	
	clear
	echo -e "   ${IRed}**********************************************${STD}"	
	echo -e "   ${IRed}*  Date : ${CURRENTDATEONLY}     *${STD}"
	echo -e "   ${IRed}*   Timezone and Synchronize System Clock    *${STD}"
	echo -e "   ${IRed}*              Bash Menu Driven              *${STD}"
	echo -e "   ${IRed}*          Written by MD Harrington          *${STD}"
	echo -e "   ${IRed}*                                            *${STD}"
	echo -e "   ${IRed}**********************************************${STD}"
	echo -e "   ${IGreen}1. Show Current Set Time Zone ${STD}"
	echo -e "   ${IPurple}2. Set Time Now {STD}"
	echo -e "   ${IBlue}3. Check Current Time and Date Status${STD}"
	echo -e "   ${IRed}4. NTP synchronized ${STD}"
	echo -e "   ${ICyan}5. Use Local TZ NTP Not Synced ${STD}"
	echo -e "   ${IWhite}6. Exit ${STD}"
}



pause(){
	
  read -p "Press [Enter] to continue..." p_EnterKey
}

GetCurrentTimeZone()
{   
	echo "Time Zone is set to "
	echo "" 
	timedatectl | grep Time
	pause
    
}


SetTimeNow()
{
   echo -e  "${IGreen}Set time  for NOW ${STD}" 
   sudo systemctl stop  systemd-timesyncd.service 
   sudo timedatectl set-local-rtc 1
   sudo timedatectl set-ntp false
   read -p "Input time duration of simulation in HH:MM:SS format " -e Time

	while :; do
		if [[ $Time =~ ^([0-9]{2}):([0-9]{2}):([0-9]{2})$ ]]; then
			if (( BASH_REMATCH[3] < 60 ))\
			&& (( BASH_REMATCH[2] < 60 ))\
			&& (( BASH_REMATCH[1] < 24 )); then
			break
		fi
	fi
   read -p "Wrong format. Please use the HH:MM:SS format " -e Time
   done

# Set Time with $Time  
 timedatectl set-time $Time
 
 echo "Self check to see if time was set correctly "
 
 timedatectl   
 
 pause 
 
 timedatectl set-ntp true
 systemctl restart  systemd-timesyncd.service 

   
}

checkstatus()
{
	echo -e  "${IPurple}Checking time and date status${STD}"
	timedatectl status 
	
	
	pause
	
}

synchNTP()
{
	echo -e  "${IYellow}Synchronize NTP${STD} " 
	sudo systemctl stop  systemd-timesyncd.service 
	sudo timedatectl set-local-rtc 0

	timedatectl set-ntp true
	
	sudo systemctl restart  systemd-timesyncd.service  
	
	echo -e "${IYellow}You may need to reboot the Raspberry Pi to apply this change${STD} " 
	timedatectl status 
	pause 
	
}

UseLocalTZ()
{
	echo -e  "${ICyan}NB !! Using Local TZ and  not synced with NTP server ${STD} " 
	sudo systemctl stop  systemd-timesyncd.service 
	sudo timedatectl set-local-rtc 1
	
	sudo systemctl restart  systemd-timesyncd.service  
	timedatectl status 
	pause 
	
}

QuitMenu()
{
	echo ""
	echo ""
	echo -e "   ${ICyan}**********************************************${STD}"
	echo -e "   ${ICyan}*      Thank you for using this menu         *${STD}"
	echo -e "   ${ICyan}*         Mark David Harrington              *${STD}"
	echo -e "   ${ICyan}**********************************************${STD}"
	
		sleep 1
		  clear 
	        exit 0 
}



read_options(){
	
	local choice
	read -p "Enter choice [ 1 - 6] " choice
	case $choice in
		1) GetCurrentTimeZone;;
		2) SetTimeNow ;;
		3) checkstatus ;;
		4) synchNTP;;
		5) UseLocalTZ ;;
		6) QuitMenu ;;
		*) echo -e "${RED}Error...${STD}" && sleep 2
	esac
}

# ----------------------------------------------
#  Trap CTRL+C, CTRL+Z and quit signals
# ----------------------------------------------

trap '' SIGINT SIGQUIT SIGTSTP
 
# -----------------------------------
# Main logic - infinite loop
# ------------------------------------
while true
do
 
	show_menus
	read_options
done

