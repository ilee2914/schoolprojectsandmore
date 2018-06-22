AUTHOR : IRA LEE
PURPOSE : To detect lines in the given grayscale images.

VALUES USED :
For converting into binary, threshold value : 50 
For hough_simple_1.pgm, threshold value : 200+
For hough_simple_2.pgm, threshold value : 185
For hough_complex_1.pgm, threshold value :185

REFLECTION :
The project itself was not too hard. I had trouble figuring out how to find local maxima. I just
looked in squares of 10 and averaged it, giving weight to pixels that are above the threshold amount.
It was not the best way, but it sure cut down the number of lines I had on the screen.

The math proved to be difficult to understand at first, but after doing the project, I am happy to say I
understand the topic much better! The lines are not perfect however. In simple1, I can't seem to find where
or why those lines are in the wrong posion when mostly ever other line is fine.