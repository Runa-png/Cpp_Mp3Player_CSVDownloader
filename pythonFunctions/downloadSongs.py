import json
import sys
from yt_dlp import YoutubeDL
from sqlite3 import connect, IntegrityError, Row
from urllib.parse import urlparse, parse_qs
from shutil import move
from pathlib import Path

def main(songData: list):
  songDataClass = songDataDownload(songData)
  
  return

class songDataDownload:
  def __init__(self, songData):
    self.allSongData = songData

    # Database connections
    self.connection = None
    self.cursor = None

    
    self.PROJECT_ROOT = Path(__file__).resolve().parent.parent
    self.dbLocation = f"{self.PROJECT_ROOT}/appData/database.db"
    self.outputLocation = f"{self.PROJECT_ROOT}/appData/cache"

    self.cookieOptions = ["firefox", "chrome", "edge", "brave", "safari"]
    self.cookieOptionsIndex = 0

    self.options = {
      "format": "bestaudio/best",
      "quiet": True,
      "no_warnings": True,
      "noprogress": True,
      "ignoreerrors": False,
      "remote_components": ["ejs:npm"],
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
    self.connection.row_factory = Row
    self.cursor = self.connection.cursor()
  
  def commitDB(self):
    self.connection.commit()
  
  def disconnectFromDB(self):
    self.connection.close()
    self.connection = None
    self.cursor = None

  def iterateOverSongsForDownload(self):
    self.connectToDB()
    
    for song in self.allSongData:      
      songName = song["Song"].replace(" ", "^") # A placeholder for spaces in song names
      albumName = song["Album"].replace(" ", "^") # A placeholder for spaces in album names
      fileName = songName + "¬" + albumName

      # If the path already exists, do not allow the download
      if self.checkFileExists(f"{self.PROJECT_ROOT}/appData/music/{fileName}.mp3"):
        responseData = {"successful": False, "code": "102 Song already exists", "index": song["#"]}
        print(json.dumps(responseData), flush=1)
        continue
      
      # Download audio, store it in appData/cache
      response = self.downloadSong(fileName=fileName, url=song["url"], index=song["#"])
      
      # Failed to make the download
      if response["break"] == True:
        responseData = {"successful": False, "code": response['response'], "index": response["index"]}
        print(json.dumps(responseData), flush=1)
        continue
      
      else:                
        # Something happened when we wrote the song to the database
        successWrite = self.writeSongToDB(fileName=fileName, songData=song) # Write song data to the DB
        if not successWrite["success"]:
          responseData = {"successful": False, "code": "101 UNIQUE constraint failed", "index": song["#"]}
          print(json.dumps(responseData), flush=1)
          continue
        
        # Something went wrong with shutil
        successfulTransfer = self.transferSong(fileName=fileName)
        if not successfulTransfer["success"]:
          responseData = {"successful": False, "code": "103 File was not able to be moved", "index": song["#"]}
          print(json.dumps(responseData), flush=1)
          continue
        
        responseData = {"successful": True, "code": "", "index": song["#"]}
        print(json.dumps(responseData), flush=1)
        self.commitDB()
    self.disconnectFromDB()
        
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
    retried = False
    while True:
      try:
        self.cursor.execute("INSERT INTO songs (songName, artistName, albumName, fileLocation) VALUES (?,?,?,?)", (songData["Song"], songData["Artist"], songData["Album"], fileName))
        return {"success": True, "reason": ""}
      except IntegrityError:
        fixComplete = self.fixIntegrityError(fileName)
        
        if fixComplete["success"] == True and not retried:
          retried = True # We'll retry adding to the database
        elif retried:
          return {"success": False, "reason": "Maximum amount of retries to insert into database"}
        else:
          return {"success": False, "reason": "We were not able to insert into database, even after attempted fix"}

  def transferSong(self, fileName):
    try:
      move(f"{self.PROJECT_ROOT}/appData/cache/{fileName}.mp3", f"{self.PROJECT_ROOT}/appData/music/{fileName}.mp3")
      return {"success": True}
    except Exception as e:
      return {"success": False, "reason": e, "query": f"{self.PROJECT_ROOT}/appData/cache/{fileName}.mp3 appData/music/{fileName}.mp3"}

  def checkFileExists(self, filePath):
    path = Path(filePath)
    if path.exists():
      return True
    else:
      return False

  def fixIntegrityError(self, filePath):
    songData = self.cursor.execute("SELECT * FROM songs WHERE fileLocation = ?", (filePath,)).fetchall()
    
    if songData:
      if self.checkFileExists(filePath):
        return {"success": False, "reason": "The mp3 exists and is in the database, this should not be possible"}
      else:
        self.connection.execute("DELETE FROM songs WHERE fileLocation = ?", (filePath,)) # The mp3 is gone, rid the database of this ghost
        return {"success": True, "reason": ""}

    else:
      return {"success": False, "reason": "There is no database entry, how did a data collision happen?"}

if __name__ == "__main__":
  if len(sys.argv) > 1:
    songData = json.loads(sys.argv[1])
    main(songData)
  else:
    data = {"successful": False, "code": "404 data not present in the request"}
    print(json.dumps(data), flush=1)
    