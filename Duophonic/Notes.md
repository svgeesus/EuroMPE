# Notes

## Overall plan

- what is PNG
- PNG 3rd edition
    - HDR support
    - animation
    - [bugfixes](https://www.w3.org/TR/png-3/#changes-20031110)
- PNG 4th edition
    - metadata (C2PA)
    - HDR & SDR updates
- PNG 5th edition
    - compression updates

## PNG

- Lossless, raster RGB(A) image format
- Full alpha transparency
- Replacement for GIF, RGB TIFF
- W3C Standard since 1996
- (usage 28%, from HTTP archive) https://almanac.httparchive.org/en/2024/media#format-adoption


## Color space identification

- Most images still sRGB https://almanac.httparchive.org/en/2024/media#image-color-spaces
    0.7% AdobeRGB
    0.4% Display P3
- ICC profiles (but can be large)
    87% images with no profile, so sRGB
    typically 3 to 5kB
    tiny profiles (c2ci is 424 bytes)
- CICP much smaller
    (4 bytes)
