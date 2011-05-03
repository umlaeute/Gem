video-plugin with a QTKit backend
=================================

input (from cam)
QTCaptureDevice

output (to pix)
QTCaptureVideoPreviewOutput
QTCaptureDecompressedVideoOutput


see also:
getPosterFrame getFrameImageAtTime

from TechNote2138: Listing 1  How to get a CGImage for a movie frame at a given time.
<snip>
 NSDictionary *dict = [NSDictionary 
                      dictionaryWithObject:QTMovieFrameImageTypeCGImageRef 
                                    forKey:QTMovieFrameImageType];
    QTTime time = [movie currentTime];

    CGImageRef theImage = (CGImageRef)[movie frameImageAtTime:time 
                                   withAttributes:dict error:NULL];
</snip>
