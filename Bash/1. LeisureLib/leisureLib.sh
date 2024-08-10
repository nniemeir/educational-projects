#!/bin/bash
# Author: Jacob Niemeir <nniemeir@protonmail.com>
# Browse media library efficiently using fzf

# Ensure configuration file is present
source preferences.conf || {
	echo "Error: No configuration file found."
	exit 1
}

if ! [ -f game_collection.csv ]; then
	echo "game_collection.csv not found."
	exit 1
fi

# Ensure fzf is installed
command -v fzf >/dev/null 2>&1 || {
	echo >&2 "Error: fzf not found"
	exit 1
}

promptMediaType() {
	clear
	mediaSections="Books\nFilms\nGames\nTelevision"
	mediaType=$(fzfPrompt "$mediaSections")
	case "$mediaType" in
	Books) promptBooks ;;
	Films) promptFilms ;;
	Games) while true; do
		promptFilter
		promptGame
	done ;;
	Television) promptTV ;;
	*)
		kitty icat --clear
		clear
		exit 0
		;;
	esac
}

fzfPrompt() {
	local libraryFiles="$1"
	# Define the preview command
	selection=$(echo -e "$libraryFiles" | xargs -I {} basename "{}" | sed 's/\.[^.]*$//' | fzf --height=80% --delimiter , --padding=5,40,0,40 --layout=reverse --cycle)
	echo $selection
}

promptBooks() {
	finished="0"
	while [ $finished == "0" ]; do
		bookFiles=$(ls "$BOOKS_PATH/"*.{epub,pdf} 2>/dev/null)
		selection=$(fzfPrompt "$bookFiles")
		if [ -z "$selection" ]; then
			promptMediaType
		else
			zathura "$BOOKS_PATH/$selection".* 2>/dev/null
		fi
	done
}

promptFilms() {
	finished="0"
	while [ $finished == "0" ]; do
		# Add any other extensions if present in your library
		filmFiles=$(ls "$MOVIES_PATH/"*.{mkv,mp4} 2>/dev/null)
		# Create the primary and preview fzf window
		selection=$(fzfPrompt "$filmFiles")
		if [ -z "$selection" ]; then
			promptMediaType
		else
			mpv "$MOVIES_PATH/$selection".* --fullscreen >/dev/null 2>&1 &
		fi
	done
}

# It is determined which supported runners are installed
enumerateRunners() {
	case $runner in
	BlastEm) flatpak list | grep "com.retrodev.blastem" ;;
	bsnes) flatpak list | grep "dev.bsnes.bsnes" ;;
	DeSmuME) flatpak list | grep "org.desmume.DeSmuME" ;;
	Dolphin) flatpak list | grep "org.DolphinEmu.dolphin-emu" ;;
	Flycast) flatpak list | grep "org.flycast.Flycast" ;;
	Heroic) command -v heroic ;;
	Lutris) command -v lutris ;;
	mGBA) flatpak list | grep "io.mgba.mGBA" ;;
	Nestopia) flatpak list | grep "ca._0ldsk00l.Nestopia" ;;
	PCSX2) flatpak list | grep "net.pcsx2.PCSX2" ;;
	PPSSPP) flatpak list | grep "org.ppsspp.PPSSPP" ;;
	RPCS3) flatpak list | grep "net.rpcs3.RPCS3" ;;
	Steam) command -v steam ;;
	*)
		echo "Error: Invalid Runner"
		exit 1
		;;
	esac
}

# The command used to launch the selected game is determined from the matching line's second field
launch() {
	case $runner in
	BlastEm)
		flatpak run com.retrodev.blastem "$MD_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	bsnes)
		flatpak run dev.bsnes.bsnes "$SNES_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	DeSmuME)
		flatpak run org.desmume.DeSmuME "$DS_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	Dolphin)
		flatpak run org.DolphinEmu.dolphin-emu "$GAMECUBE_WII_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	Flycast)
		flatpak run org.flycast.Flycast "$DREAMCAST_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	Heroic)
		xdg-open heroic://launch/legendary/"$gameID" >/dev/null 2>&1 &
		;;
	Lutris)
		env LUTRIS_SKIP_INIT=1 lutris "lutris:rungameid/$gameID" >/dev/null 2>&1 &
		;;
	mGBA)
		flatpak run io.mgba.mGBA "$GBA_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	Nestopia)
		flatpak run ca._0ldsk00l.Nestopia "$NES_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	PCSX2)
		flatpak run net.pcsx2.PCSX2 "$PS2_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	PPSSPP)
		flatpak run org.ppsspp.PPSSPP "$PSP_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	RPCS3)
		flatpak run net.rpcs3.RPCS3 "$PS3_ROMS/$gameID" >/dev/null 2>&1 &
		;;
	Steam)
		steam steam://rungameid/"$gameID" >/dev/null 2>&1 &
		;;
	*)
		echo "The runner $runner is not currently supported"
		;;
	esac
}

promptFilter() {
	finished="0"
	while [ $finished == "0" ]; do

		filter=$(fzfPrompt "$availableRunnersDisplay")
		# If input not given for filter, exit script
		if [ -z "$filter" ]; then
			clear
			promptMediaType
		fi

		# If filter is All, the variable is emptied
		if [ "$filter" == "All" ]; then
			filter=""
		fi

		# Each line that fits our filter criteria is saved to this variable
		filtered=$(awk -v filter="$filter" -v availableRunners="$availableRunners" 'BEGIN { FS = "," } {
	if (filter == "") {
		if (index(availableRunners, $2) > 0) {
		print $1;
	}
	}
else {
	split(filter, runners, /\n/);
	for (i in runners) {
		if ($2 == runners[i]) {
		print $1;
	   }
	}
}
}' game_collection.csv)

		clear
		break
	done
}

promptGame() {
	finished="0"
	while [ $finished == "0" ]; do
		selection=$(fzfPrompt "$filtered")

		# The loop breaks if no game is selected
		if [ -z "$selection" ]; then
			break
		fi

		# Grab the other fields of the line in our collection whose first field is the same as the selected game
		runner=$(awk 'BEGIN { FS = "," } /'"$selection"'/ { print $2 }' game_collection.csv)
		gameID=$(awk 'BEGIN { FS = "," } /'"$selection"'/ { print $3 }' game_collection.csv)

		# Launch the selected game
		launch

		clear
	done
}

SUPPORTED_RUNNERS="BlastEm\nbsnes\nDeSmuME\nDolphin\nFlycast\nHeroic\nLutris\nmGBA\nNestopia\nPCSX2\nPPSSPP\nRPCS3\nSteam"

unavailableRunners=""
for runner in $(echo -e "$SUPPORTED_RUNNERS"); do
	if [[ ! $(enumerateRunners "$runner") ]]; then
		unavailableRunners="$unavailableRunners$runner"
	fi
done

# If a runner in SUPPORTED_RUNNERS is installed, it is appended to availableRunners
availableRunners=""
for runner in $(echo -e "$SUPPORTED_RUNNERS"); do
	if [[ ! "$unavailableRunners" =~ "$runner" ]]; then
		availableRunners="$availableRunners$runner\n"
	fi
done

# Strip trailing newline and prepend "All" option before creating fzf windows
availableRunners=$(echo -e "$availableRunners" | sed -e '$!b' -e '/^\n*$/d')
availableRunnersDisplay="All\n$availableRunners"


promptTV() {
	clear
	shows=$(ls $SHOWS_PATH)
	showSelection=$(fzfPrompt "$shows")

	export showSelection

	if [ -z "$showSelection" ]; then
		promptMediaType
	else
		promptSeason
	fi
}

promptSeason() {
	clear
	seasons=$(ls "$SHOWS_PATH/$showSelection")
	seasonSelection=$(fzfPrompt "$seasons")
	if [ -z "$seasonSelection" ]; then
		promptTV
	else
		promptEpisode
	fi
}

promptEpisode() {
	finished="0"
	while [ $finished == "0" ]; do
		clear
		episodeFiles=$(ls "$SHOWS_PATH/$showSelection/$seasonSelection/"*.{mkv,mp4} 2>/dev/null)
		episodeSelection=$(fzfPrompt "$episodeFiles")
		if [ -z "$episodeSelection" ]; then
			promptSeason
		else
			mpv "$SHOWS_PATH/$showSelection/$seasonSelection/$episodeSelection".* --fullscreen >/dev/null 2>&1 &
		fi
	done
}

preview_file=""

promptMediaType
kitty icat --clear
clear
exit 0
