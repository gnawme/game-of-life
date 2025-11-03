#!/bin/bash
# record_game.sh - Record Game of Life with ffmpeg

# Usage: ./record_game.sh [duration] [pattern] [game_options...]
# Examples:
#   ./record_game.sh 15 patterns/pulsar.cells
#   ./record_game.sh 15 patterns/glider.rle --wrapped
#   ./record_game.sh 20 --random --wrapped

DURATION="${1:-15}"
shift  # Remove duration from arguments

# Check if first remaining arg is a pattern file or an option
if [[ "$1" == --* ]]; then
    # No pattern specified, just options
    PATTERN=""
    GAME_OPTIONS="$*"
else
    # Pattern specified
    PATTERN="$1"
    shift
    GAME_OPTIONS="$*"
fi

# Generate output name based on pattern/options
if [ -n "$PATTERN" ]; then
    BASENAME=$(basename "$PATTERN" | sed 's/\.[^.]*$//')
    OUTPUT="${BASENAME}_$(date +%s).gif"
else
    OUTPUT="random_$(date +%s).gif"
fi

echo "=== Game of Life Recording Script ==="
echo "Duration: ${DURATION}s"
echo "Pattern: ${PATTERN:-random}"
echo "Options: ${GAME_OPTIONS:-none}"
echo "Output: $OUTPUT"
echo ""

# Fix DISPLAY variable
export DISPLAY=${DISPLAY:-:0}
echo "Using DISPLAY: $DISPLAY"

# Get screen info
SCREEN_RESOLUTION=$(xdpyinfo | grep dimensions | awk '{print $2}' | cut -d'x' -f1,2)
SCREEN_WIDTH=$(echo "$SCREEN_RESOLUTION" | cut -d'x' -f1)
SCREEN_HEIGHT=$(echo "$SCREEN_RESOLUTION" | cut -d'x' -f2)

echo "Screen resolution: ${SCREEN_WIDTH}x${SCREEN_HEIGHT}"

# Game window is 1280x720, calculate center position
GAME_WIDTH=1280
GAME_HEIGHT=720
OFFSET_X=$(( (SCREEN_WIDTH - GAME_WIDTH) / 2 ))
OFFSET_Y=$(( (SCREEN_HEIGHT - GAME_HEIGHT) / 2 ))

echo "Game window will be centered at: ${OFFSET_X},${OFFSET_Y}"
echo ""
echo "Starting in 3 seconds..."
sleep 3

# Build game command
if [ -n "$PATTERN" ]; then
    # shellcheck disable=SC2086
    eval "./build/game_of_life \"$PATTERN\" $GAME_OPTIONS" &
    GAME_PID=$!
else
    # shellcheck disable=SC2086
    eval "./build/game_of_life $GAME_OPTIONS" &
    GAME_PID=$!
fi

echo "Game launched with PID: $GAME_PID"

# Give game time to open and position itself
sleep 2

# Start recording the centered 1280x720 region
echo "Recording started (${DURATION}s)..."
ffmpeg -f x11grab -r 30 -s "${GAME_WIDTH}x${GAME_HEIGHT}" \
  -i "${DISPLAY}+${OFFSET_X},${OFFSET_Y}" \
  -t "$DURATION" \
  "/tmp/game_recording_$$.mp4" -y 2>&1 | grep -E "frame=|error" &
FFMPEG_PID=$!

# Wait for recording to finish
wait $FFMPEG_PID

# Kill game if still running
if ps -p $GAME_PID > /dev/null; then
    echo "Stopping game..."
    kill $GAME_PID 2>/dev/null
    wait $GAME_PID 2>/dev/null
fi

# Check if recording was created
if [ ! -f "/tmp/game_recording_$$.mp4" ]; then
    echo "ERROR: Recording failed!"
    exit 1
fi

echo "Recording complete. Converting to GIF..."

# Convert to optimized GIF
ffmpeg -i /tmp/game_recording_$$.mp4 \
  -vf "fps=15,scale=640:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" \
  -loop 0 "$OUTPUT" -y 2>&1 | grep -E "frame=|error"

if [ ! -f "$OUTPUT" ]; then
    echo "ERROR: GIF conversion failed!"
    rm /tmp/game_recording_$$.mp4
    exit 1
fi

echo "Optimizing GIF..."
gifsicle -O3 --colors 256 "$OUTPUT" -o "${OUTPUT%.gif}_optimized.gif"

if [ -f "${OUTPUT%.gif}_optimized.gif" ]; then
    mv "${OUTPUT%.gif}_optimized.gif" "$OUTPUT"
fi

rm /tmp/game_recording_$$.mp4

echo ""
echo "✓ Done! Saved as: $OUTPUT"
ls -lh "$OUTPUT"

# Show file size
SIZE=$(du -h "$OUTPUT" | cut -f1)
echo "File size: $SIZE"
