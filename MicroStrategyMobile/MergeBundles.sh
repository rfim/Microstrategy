#!/bin/bash

# 
# Script to merge custome resources into target bundle
# Argument 1: The target bundle name.
# Argument 2: The custom bundle location.
# Argument 3: Framework Root path
# 

# Log file
LOG_FILE=$SRCROOT/MergeBundles.log.txt

# App bundle path
APP_WRAPPER_PATH="$BUILT_PRODUCTS_DIR/$WRAPPER_NAME"

# Final Bundle name
FINAL_BUNDLE_NAME="$1"

# Destination Resource Path
DEST_RESOURCE_PATH="$APP_WRAPPER_PATH/$FINAL_BUNDLE_NAME/Contents/Resources"

# Custom Resource path. 
CUSTOM_RESOURCE_PATH="$2"

# Merge Custom bundle resources into target bundle
[ -d "$CUSTOM_RESOURCE_PATH/Contents/Resources/" ] && [ '$(ls -A "$CUSTOM_RESOURCE_PATH/Contents/Resources/")' ] && cp -f -R "$CUSTOM_RESOURCE_PATH/Contents/Resources/" "$DEST_RESOURCE_PATH" && echo "Files copied from $CUSTOM_RESOURCE_PATH" >> $LOG_FILE

# Merge Plists
FRAME_WORK_ROOT="${3:-../Library/MicroStrategyMobileSDK.framework/}"
ORG_RESOURCE_PATH="$FRAME_WORK_ROOT/"
TMP_FILE="./tmp.plist"
PLIST_UTIL="/usr/libexec/PlistBuddy"
if [ -d $FRAME_WORK_ROOT ]; then
# Travese the Custom Bundle to find files with ext .plist
    for plist in $(find "$CUSTOM_RESOURCE_PATH" -iname \*.plist) 
    do

        # Get File Name here
        PLIST_FILE_NAME=${plist##*/}

        echo $PLIST_FILE_NAME

        ORG_PLIST_PATH=$(find "$ORG_RESOURCE_PATH" -iname $PLIST_FILE_NAME | head -1)
        DEST_PLIST_PATH=$(find "$DEST_RESOURCE_PATH" -iname $PLIST_FILE_NAME | head -1)

        # Print Out Info
        echo "Merging $ORG_PLIST_PATH"
        $PLIST_UTIL -c "print" $ORG_PLIST_PATH
        echo "and $DEST_PLIST_PATH"
        $PLIST_UTIL -c "print" $DEST_PLIST_PATH

        $PLIST_UTIL -c "merge $ORG_PLIST_PATH" "$DEST_PLIST_PATH"
        echo "result $DEST_PLIST_PATH"
        $PLIST_UTIL -c "print" $DEST_PLIST_PATH

    done
else
    echo "Can't find framework $FRAME_WORK_ROOT, skip merge plis files"
fi

# Filter out unnecessary/unsupported files 
find "$APP_WRAPPER_PATH" -type f -name ".copyarea.db" -print0 | xargs -0 -I CC_FILES rm -f CC_FILES

echo "done" >> $LOG_FILE
exit 0

