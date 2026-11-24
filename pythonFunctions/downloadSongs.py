import json
import sys
from yt_dlp import YoutubeDL
from sqlite3 import connect
from urllib.parse import urlparse, parse_qs

def main(songData: list):
  songDataClass = songDataDownload(songData)
  
  return

class songDataDownload:
  def __init__(self, songData):
    self.allSongData = songData

    # Database connections
    self.connection = None
    self.cursor = None

    self.outputLocation = "appData/cache"
    self.dbLocation = "appData/database.db"

    self.cookieOptions = ["firefox", "chrome", "edge", "brave", "safari"]
    self.cookieOptionsIndex = 0

    self.options = {
      "format": "bestaudio/best",
      "quiet": True,
      "postprocessors": [
        {
          "key": "FFmpegExtractAudio",
          "preferredcodec": "mp3",
          "preferredquality": "192",
        }
      ],
    }

    self.errorCodes = [
      {"code": "is not a valid URL", "break": True, "response": "001 The URL is not valid!"},
      {"code": "positional arguments but", "break": True, "response": "002 Malformed URL"},
      {"code": "No host supplied", "break": True, "response": "003 URL has no hostname"},
      {"code": "Playlists that require authentication may not extract correctly without a successful webpage download", "break": True, "response": "404 Location not found or video is private"},
      {"code": "Incomplete YouTube ID", "break": True, "response": "004 Incomplete URL ID"},
      {"code": "YouTube said: This playlist type is unviewable", "break": True, "response": "Remove the playlist URL"},
      {"code": "ffprobe and ffmpeg not found", "break": True, "response": "Please install FFMPEG"}
    ]
    
    # Put Event loop here
    self.filterUnsearched() # Remove the unsearched items
    self.iterateOverSongsForDownload() # Download the music
  
  def filterUnsearched(self):
    for item in self.allSongData:
      if not "url" in item.keys():
        self.allSongData.pop(self.allSongData.index(item))
  
  def connectToDB(self):
    self.connection = connect(self.dbLocation)
    self.cursor = connection.cursor()
  
  def commitDB(self):
    self.connection.commit()
  
  def disconnectFromDB(self):
    self.connection.close()
    self.connection = None
    self.cursor = None

  def iterateOverSongsForDownload(self):
    for song in self.allSongData:      
      songName = song["Song"].replace(" ", "^") # A placeholder for spaces in song names
      albumName = song["Album"].replace(" ", "^") # A placeholder for spaces in album names

      fileName = songName + "¬" + albumName + ".mp3"
      
      response = self.downloadSong(fileName=fileName, url=song["url"], index=song["#"])
      
      if response["break"] == True:
        responseData = {"successful": False, "code": response['response'], "index": response["index"]}
        print(json.dumps(responseData), flush=1)
      
      else:
        print("Successful Download")
        self.writeSongToDB(fileName=fileName, songData=song)
  
  def downloadSong(self, fileName, url, index):
    self.options["cookiesfrombrowser"] = (self.cookieOptions[self.cookieOptionsIndex],)
    self.options["outtmpl"] = f'{self.outputLocation}/{fileName}'
    with YoutubeDL(self.options) as ydl:
      isVideo = self.checkIsVideo(ydl=ydl, url=url)

      if not isVideo: # Item is a playlist >:(
        return {"code": "", "break": True, "response": "Get rid of the playlist URL.", "index": index}
      
      while True: # Break if the error code requires it
        try:
          ydl.download(url)
          return {"code": "", "break": False, "response": "Success", "index": index}
        except Exception as e:
          errorCodeData = self.findErrorCode(e)
          errorCodeData["index"] = index

          print("Exception", e)
          
          if errorCodeData["break"] == True:
            break
      return errorCodeData
  
  def checkIsVideo(self, ydl, url):
    parsed = urlparse(url)
    hostname = parsed.hostname or ""
    path = parsed.path
    params = parse_qs(parsed.query)

    if "youtube.com" in hostname:
      # Only accept videos
      if path in ["/watch"] and "v" in params and not "list" in params:
        return True

    return False
  
  def findErrorCode(self, error):
    error = str(error)

    for errorCode in self.errorCodes:
      if errorCode["code"] in error:
        return errorCode
    
    return {"code": "Unrecognised error", "break": False, "response": "Unrecognised error"}

  def writeSongToDB(self, fileName, songData):
    print("Writing Song to db")
    print(fileName)

if __name__ == "__main__":
  if len(sys.argv) > 1:
    songData = json.loads(sys.argv[1])
    main(songData)
  else:
    data = {"response": False, "reason": "FATAL: data not present in the request"}
    print(json.dumps(data), flush=1)
    