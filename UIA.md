# UIA (UNIX-based Image Analyst)
Written and developed by: Dmitriy Bernsteyn, Suzanne Curry, Craig Holmquist, Robert Pollock, Mark Schmalz, and Matthew Weingarten
## Purpose
The purpose of UIA is to be able to combine, analyze, and manipulate images from the command line on UNIX systems. No longer are GUI frameworks, which are difficult to work with and/or expensive, necessary as UIA can do everything they can do, but in a much simpler manner.
## How to Use
UIA relies on shell commands and built-in tools such as ImageMagick for its successful use. If the location of these libraries are different on your machine than they are on the ones in which this was developed, then those lines of code will need to be updated before running the program, or else those routines will not run correctly (if they run at all). Make sure that you have checked this and verified that they are right before running for the first time.

To run UIA, type in ./img.exe, the operation you want to perform, and the necessary arguments for that operation. What are the commands, what are the arguments they take in, and what do they produce? That is answered below.
## Commands
### Setting Up
No matter what command is entered, the starting procedure is the same. First comes argument checking, making sure that the correct number of arguments are entered for the corresponding command and making sure that they are in a correct format. After that comes image conversion. All images in UIA are converted to Portable Graymap Format (PGM). PGM is a useful format in terms of working with image manipulation operations.

After converting the image(s) to PGM, the images are read into their double arrays. If two images are used for the operation, it's necessary to take the common area of those two images so that the operation can overlay them. The resize_img method takes the image where the dimension of the image (row or column) is bigger and shrinks it to the size of the smaller dimension image.

When all of that is complete, the individual operations are run.
### imgadd
Takes in: two images\
Produces: one output image

imgadd uses the "plus" option in ImageMagick's composite method to add the two images.
### imgsub
Takes in: two images\
Produces: one output image

imgsub uses the "subtract" option in ImageMagick's composite method to subtract the two images.
### imgmul
Takes in: two images\
Produces: one output image

imgmul multiplies the two image arrays pixel-by-pixel to produce its output.
### imgdiv
Takes in: two images\
Produces: one output image

imgdiv divides the two image arrays pixel-by-pixel to produce its output. If one of the pixels in the second image array is 0, it sets the corresponding pixel in the output image to 0 to avoid a divide-by-zero error.
### imgsin
Takes in: one image\
Produces: one output image

imgsin runs a sine transformation on the image array. Values are normalized to pi/2 whenever necessary and then further normalized to 1-255.
### imglog
Takes in: one image\
Produces: one output image

imglog runs a log base 10 transformation on the image array. Values are normalized to 1-255 whenever necessary.
### gray
Takes in: one image\
Produces: one output image

gray turns an image into a gray scale image.
### thold
Takes in: one image and a threshold value\
Produces: one output image

thold converts a regular image to a monochrome image based on the threshold value. If the image value is greater than the threshold, then the pixel value is set to white and is set to black otherwise. An all-zero array is not allowed.
### comp
Takes in: one image\
Produces: one output image

comp changes the background of an image to a new color that is determined from other methods.
### ftruth
Takes in: two images\
Produces: tabular output

ftruth determines the number of correct hits, false alarms, correct misses, and miss detections for two monochrome images. This is represented in a truth table format.
### ctruth
Takes in: two images\
Produces: text output

ctruth is very similar to ftruth, but doesn't output the truth table that ftruth does.
### wht
Takes in: one image\
Produces: one output image

wht runs the Walsh-Hadamard transformation on an image. It is very similar to fft in that the results are displayed using ImageMagick's display routine.
### gcon, amax, amin, mmax, and mmin
Takes in: one image, a 3x3 or 5x5 list of integers, and the size of the file to be produced.\
Produces: one output image

These routines run geometric and linear convolutions on an image array.
### rmse
Takes in: two images\
Produces: text output

rmse computes the root mean square error (RMSE) for all four quadrants of the difference of the two images as well as the whole difference image.
### rmsegrid
Takes in: two images and a number of rows and columns\
Produces: tabular output

rmsegrid runs rmse and produces its output as a table on the screen, breaking the output up into multiple tables if there are more than 10 columns.
### rmsegrid01
Takes in: two images and a number of rows and columns\
Produces: tabular output

rmsegrid01 normalizes the RMSE array based on the min/max from both of the input images. It then displays the result in a table format similar to rmsegrid.
### rmseann
Takes in: two images and a number of rings\
Produces: text output

rmseann produces an annular RMSE table based on a number of rings
### fft
Takes in: an image and the root of the output file\
Produces: an output file

fft runs the Fast Fourier transformation on an image and shows its results using ImageMagick's display routine.
### fftspect
Takes in: an image and an output file\
Produces: an output file

fftspect runs graph programs on the result of fft to look at its spectrometry.
### help
Takes in: nothing\
Produces: nothing

help displays the format of every command currently available in UIA.
### Cleaning Up
After running the command, UIA removes all temp files and frees unnecessary image arrays. The code picks up after itself.
## For More Information
The code is well-commented in most areas. For more technical information, look at the comments corresponding to the command you are trying to run. And when that isn't enough, try researching the transformation and how it is typically implemented in code. What we do here is nothing out of the ordinary, so background research will have you well-acquainted to work with the code.
