#!/bin/bash

boolAll=false
boolHelp=false #Help
boolSet=false #Set Photos folder
bool1=false #Fotos 2019
bool2=false #Fotos 2020
bool3=false #Paisajes
bool4=false #Documentos
bool5=false #Escritorios
bool6=false #Mis Imagenes
bool7=false #Files 2017
bool8=false #Files 2018
boolFiles=false #Set Folder

#Extras
boolUpdate=false
#--ignore-existing: flag to prevent files from being copied over that already exist on the remote server.
#--update: Any files that exist on both local and remote that have a newer timestamp on the local server are copied over. 

#Check all the Arguments
for i in "$@"
do
   if [ "$i" = "-h" ]; then boolHelp=true; fi
   if [ "$i" = "-a" ]; then boolAll=true; fi
   if [ "$i" = "-s" ]; then boolSet=true; fi
   if [ "$i" = "-1" ]; then bool1=true; fi
   if [ "$i" = "-2" ]; then bool2=true; fi
   if [ "$i" = "-3" ]; then bool3=true; fi
   if [ "$i" = "-4" ]; then bool4=true; fi
   if [ "$i" = "-5" ]; then bool5=true; fi
   if [ "$i" = "-6" ]; then bool6=true; fi
   if [ "$i" = "-7" ]; then bool7=true; fi
   if [ "$i" = "-8" ]; then bool8=true; fi
   if [ "$i" = "-f" ]; then boolFiles=true; fi
   if [ "$i" = "-u" ]; then boolUpdate=true; fi
done

#Copy all data
if [ "$boolAll" == true ]; then
   clear
   echo "Sending All"
   bool1=true
   bool2=true
   bool3=true
   bool4=true
   bool5=true
   bool6=true
   bool7=true
   bool8=true
fi

#-------------------------------------------------------------
#Help
#-------------------------------------------------------------
if [ "$boolHelp" = true ] || [ "$#" == 0 ] ; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
echo -e "\e[37mScript eduBackUp to send files in HardDisk:"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\t[-a]: \tCopy all the files"
   echo -e "\t[-h]: \tHelp"
   echo -e "\t[-s Folder]: \tCopy 'FolderName' into fotos folder"
   echo -e "\t[-1]: \tCopy Fotos 2019"
   echo -e "\t[-2]: \tCopy Fotos 2020"
   echo -e "\t[-3]: \tCopy Paisajes"
   echo -e "\t[-4]: \tCopy Documents"
   echo -e "\t[-5]: \tCopy Escritorio"
   echo -e "\t[-6]: \tCopy Mis imagenes"
   echo -e "\t[-7]: \tCopy Files 2017"
   echo -e "\t[-8]: \tCopy Files 2018"
   echo -e "\t[-f Folder]: \tCopy 'FolderName' from files"
   echo -e "\t----- Extras -----"
   echo -e "\t[-u]: \tUpdate Files"
   echo -e " "
   echo -e "Examples:"
   echo -e "\tmakeBackupEdu -f E2i"
   echo -e "\tmakeBackupEdu -s 'Fotos 2020'"
   echo -e " "
   exit 0
fi

#------------------------------------------------------------
#Set Photos Folder Name
#------------------------------------------------------------
if [ "$boolSet" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending /$2"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Local_G/Fotos/$2/"
   DST="/media/edu/eDu/fotos/$2/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   if [ "$boolUpdate" = true ]; then
      rsync --update -avhz --progress "$SRC" "$DST"
   else
      rsync --ignore-existing -avhz --progress "$SRC" "$DST"
   fi
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
   exit 0
fi


#------------------------------------------------------------
#Sending Fotos 2019
#------------------------------------------------------------
if [ "$bool1" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
	echo -e "\e[37m\t\tSending Fotos 2019"
	for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo

   SRC="/home/edu/Local_G/Fotos/Fotos 2019/"
   DST="/media/edu/eDu/fotos/Fotos 2019/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   if [ "$boolUpdate" = true ]; then
      rsync --update -avhz --progress "$SRC" "$DST"
   else
      rsync --ignore-existing -avhz --progress "$SRC" "$DST"
   fi
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi
#------------------------------------------------------------

#------------------------------------------------------------
#Sending Fotos 2020
#------------------------------------------------------------
if [ "$bool2" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending Fotos 2020"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Local_G/Fotos/Fotos 2020/"
   DST="/media/edu/eDu/fotos/Fotos 2020/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   if [ "$boolUpdate" = true ]; then
      rsync --update -avhz --progress "$SRC" "$DST"
   else
      rsync --ignore-existing -avhz --progress "$SRC" "$DST"
   fi
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi
#------------------------------------------------------------

#------------------------------------------------------------
#Sending Paisajes
#------------------------------------------------------------
if [ "$bool3" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending Paisajes"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Local_G/Fotos/Paisajes/"
   DST="/media/edu/eDu/fotos/Paisajes/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   if [ "$boolUpdate" = true ]; then
      rsync --update -avhz --progress "$SRC" "$DST"
   else
      rsync --ignore-existing -avhz --progress "$SRC" "$DST"
   fi
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi
#------------------------------------------------------------

#------------------------------------------------------------
#Sending Documents
#------------------------------------------------------------
if [ "$bool4" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending Documents "
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Documents/"
   DST="/media/edu/eDu/Mis documentos/EDUARDO/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   START="/home/edu/Documents/Starcraft"
   STARTII="/home/edu/Documents/StarCraft II"
   echo "Excluding..." $START
   echo "Excluding..." $STARTII
   rsync --update -avhz --progress --exclude 'Starcraft' --exclude 'StarCraft II' "$SRC" "$DST"
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi
#------------------------------------------------------------

#------------------------------------------------------------
#Sending Escritorio
#------------------------------------------------------------
if [ "$bool5" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending Escritorio"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Local_G/Escritorio/"
   DST="/media/edu/eDu/Local Disk G/Escritorio/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   rsync --update -avhz "$SRC" "$DST"
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi
#------------------------------------------------------------

#------------------------------------------------------------
#Sending Mis Imagenes
#------------------------------------------------------------
if [ "$bool6" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending Mis Imagenes"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Pictures/"
   DST="/media/edu/eDu/Mis documentos/Mis imagenes/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   if [ "$boolUpdate" = true ]; then
      rsync --update -avhz --progress "$SRC" "$DST"
   else
      rsync --ignore-existing -avhz --progress "$SRC" "$DST"
   fi
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi
#------------------------------------------------------------

#------------------------------------------------------------
#Sending 2017
#------------------------------------------------------------
if [ "$bool7" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending 2017"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Local_G/2017/"
   DST="/media/edu/eDu/Local Disk G/2017/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   rsync --update -avhz --progress "$SRC" "$DST"
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi
#------------------------------------------------------------

#------------------------------------------------------------
#Sending 2018
#------------------------------------------------------------
if [ "$bool8" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending 2018"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Local_G/2018/"
   DST="/media/edu/eDu/Local Disk G/2018/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   rsync --update -avhz --progress "$SRC" "$DST"
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi

#------------------------------------------------------------
#Sending Files
#------------------------------------------------------------
if [ "$booliFiles" = true ]; then
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   echo -e "\e[37m\t\tSending /$2"
   for i in {16..21} {21..16}; do for j in {1..4}; do echo -en "\e[38;5;${i}m#\e[0m" ; done;  done ; echo
   SRC="/home/edu/Local_G/$2/"
   DST="/media/edu/eDu/Local Disk G/$2/"
   echo "Source:" $SRC
   echo "Destination:" $DST
   echo "Sending . . ."
   rsync --update -avhz --progress "$SRC" "$DST"
   echo -e "\e[92m---------------------->[done!]<--------------------\e[39m "
fi

printf "Finished!!\n"
