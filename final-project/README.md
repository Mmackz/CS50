# CS50 Final Project: NOTE50
#### Video Demo: https://youtu.be/Br45s7LBcKs
#### Description: 
My CS50 final project I have built is a note taking Chrome Extension.
I have built this extension from scratch using HTML, CSS and vanilla javascript.
These are skills that I have picked up while doing the CS50 course as well as some
web development tutorials. I have chosen this project specifically because it is
something I could personally use while learning web development and it was something 
I could achieve with my skill level as a new developer with limited experience 
outside of this course. I felt it was a great way to showcase the knowledge and 
skills I have learned throughout this course by building a dynamic web app 
using vanilla javascript.

The app is quite simple, a user will visit the page they want to save a note from.
When a user saves a note, the URL of the page will be saved with the note by use 
of the google tabs api. A short title is mandatory to leave with each note. 
A longer comment or description can also be optionally left. You can put as many 
notes as you want into each of the folders. Folders can be added and removed 
to keep notes organized.

Notes are saved using the google storage API. The storage is persistant and unique to each 
user which means the data remains if you close the browser or rest the computer. 
You can access the notes by clicking on their respective folders. Each note can be removed 
by pressing the red X in the top left corner. Once notes are saved, the user can quickly 
and easily access important notes and web pages. 

The styling for the app was mainly achieved using bootstrap classes and some flexbox techniques I 
have learned in recent weeks. I have learned quite a bit during the CS50x course, and I am 
looking forward to continuing my web development journey. Thank you CS50 staff 
for providing us with this wonderful resource for learning computer science.

#### File Descriptions:
  - **background.js** - Sets up chrome storage on first run. Sets notes to empty array and creates miscellaneous folder.
  - **manifest.json** - Configuration file for the chrome extension. This is required for a chrome extension. 
Data such as the name of the app, version and which permissions is needed is included in this file.
  - **notes.html** - HTML markup for the notes page. Notes will be dynamically rendered inside the div with the id of "notes". 
Imports Cabin font from google fonts.
  - **notes.js** - Loads and renders folders and grabs notes from storage and renders them dynamically when a folder is clicked.
  - **popup.css** - CSS stylesheet.
  - **popup.html** - Main HTML markup for the extension. Form and Buttons styled using bootstrap classes. 
  - **popup.js** - Event Listeners setup to get/remove data from google storage api. Dynamically renders the form. 
Buttons and dropdown are rendered dynamically depending on if a new folder is being input.

*Icon courtesy of https://vexels.com*
