#include "x1_disk_seek.h"
#include "x1_fdc_stepIn.h"
#include "x1_fdc_stepOut.h"
#include "x1_fdc_restore.h"
#include "x1_fdc_seek.h"

void
x1_diskSeek(const u8 destinationTrackNo, const u8 currentTrackNo)
{
    if(currentTrackNo != destinationTrackNo) {
        if((currentTrackNo + 1) == destinationTrackNo) {
            x1_fdcStepIn();
        } else if(currentTrackNo == (destinationTrackNo + 1)) {
            x1_fdcStepOut();
        } else if(destinationTrackNo == 0) {
            x1_fdcRestore();
        } else {
            x1_fdcSeek(destinationTrackNo, currentTrackNo);
        }
    }
}
