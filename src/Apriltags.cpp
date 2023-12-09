/**
 * Class for finding the apriltags.
*/
#include <Arduino.h>
#include <Wire.h>
#include <openmv.h>
#include "Apriltags.h"

void AprilTags::Init() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin();
    Wire.setClock(100000ul);
}
/**
 * returns the april tag id that it has found
 * return -1 if none found
*/
int AprilTags::FindAPs() {
    uint8_t tagCount = camera.getTagCount();
    if(tagCount) {
        AprilTagDatum tag;
        if(camera.readTag(tag)) {
            return tag.id;
        }
    }
    //return -1 if we can't find any tags.
    return -1;
}