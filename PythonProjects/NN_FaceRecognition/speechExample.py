#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Import the required module for text
# to speech conversion
import os
from gtts import gTTS

# The text that you want to convert to audio
#mytext = '¡Hi Edu!'
mytext = 'Es hora de dormir.'

class Speech(object):
    def __init__(self):
            # while True:
        try:
            #a = raw_input("What you want me to say: ")
            tts = gTTS(text=mytext, lang="es", slow=False)

            # Saving the converted audio in a mp3 file named
            testfile = "/tmp/temp.mp3"
            tts.save(testfile)

            # Playing the converted file
            os.system("mpg321 /tmp/temp.mp3")

            # Removing file
            os.system("clear")
            os.system("rm %s" % (testfile))
        except UnicodeDecodeError:
            print("Some characters are not supported.")


spc = Speech()
