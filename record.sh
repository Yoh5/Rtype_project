#!/bin/bash

SCREEN_AREA="full"

OUTPUT_FILE="$HOME/game_record_$(date +%Y%m%d%H%M%S).mp4"

if [ "$SCREEN_AREA" == "full" ]; then
    SCREEN_RESOLUTION=$(xrandr | grep '*' | awk '{print $1}')
    ffmpeg -f x11grab -s $SCREEN_RESOLUTION -i :0.0 -c:v libx264 -qp 0 -preset ultrafast $OUTPUT_FILE
else
    ffmpeg -f x11grab -s $SCREEN_AREA -i :0.0+${OFFSET_X},${OFFSET_Y} -c:v libx264 -qp 0 -preset ultrafast $OUTPUT_FILE
fi

echo "Recording started: $OUTPUT_FILE"
