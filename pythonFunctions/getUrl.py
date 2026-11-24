import sys
import json
from yt_dlp import YoutubeDL
from fuzzywuzzy import fuzz
from math import floor

def main(songData):
  songDataJson = json.loads(songData)
  functionObject = songInformation(songDataJson)
  
class songInformation:
  def __init__(self, songData):
    self.songData = songData
    self.requiredKeys = ["#", "Album", "Artist", "Spotify Track Id", "Song"] # List of needed keys

    self.options = {
      "quiet": True,
      "nowarnings": True,
      "noplaylist": True,
      "extract_flat": True
    }

    self.start()
  
  def start(self):
    self.filterKeys() # Just remove all of the not needed keys
    self.compoundedFindUrl() # Add url to the song dicts
  
  def filterKeys(self):    
    newList = []

    for song in self.songData:
      filteredSongData = {}
      
      for key in self.requiredKeys:
        filteredSongData[key] = song[key]
      
      newList.append(filteredSongData)
    
    self.songData = newList
  
  def sortResults(self, songData: dict, responseData: list):
    for response in responseData: # Iterate over each reponse from yt-dlp
      data = response.get("entries", [])

      if not data: # No audio response
        return None
      
      ratioData = []
      for instance in data: # Each result the search found
        resultTitle = instance.get("title")
        resultArtist = instance.get("uploader")
        resultID = instance.get("id")

        # Calculate accuracy for each video
        titleAccuracy = fuzz.ratio(resultTitle, songData["Song"])
        artistAccuracy = fuzz.ratio(resultArtist, songData["Artist"])
        totalRatio = floor((titleAccuracy + artistAccuracy) / 2)

        songRatioData = {"id": resultID, "ratio": totalRatio}
        ratioData.append(songRatioData)
      
    # Find the highest result
    topResult = {"ratio": -1}
    for result in ratioData:
      if result["ratio"] > topResult["ratio"]:
        topResult = result
        
    return topResult

  def compoundedFindUrl(self):
    with YoutubeDL(self.options) as ydl:
      for songData in self.songData: # Each songs dict of data
        urlResult = []
        
        for artist in songData["Artist"].split(","): # Some songs have multiple artists, iterate over them
          query = f"{songData["Song"]} {artist}"

          results = ydl.extract_info(
            f"ytsearch5:{query}",
            download = False
          )

          urlResult.append(results)

        topChoice = self.sortResults(songData, urlResult)

        songData["url"] = "https://www.youtube.com/watch?v=" + topChoice["id"]
        songData["ratio"] = topChoice["ratio"]
        songData["selectedName"] = songData["Song"]

        print(json.dumps(songData), flush=1)


if __name__ == "__main__":
  if len(sys.argv) > 1:
    main(sys.argv[1])
  else:
    print("No data provided")