from pathlib import Path
import sys
import json

import csv

## C++ & Python are silly and don't like importing things properly
projectRoot = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(projectRoot))

def main(location):
  songDataClass = songData(location)
  songDataClass.readCSV()
  
  for song in songDataClass.songData:
    print(json.dumps(song), flush=1)

class songData():
  def __init__(self, location):
    self.location = location
    self.songData = []
  
  def readCSV(self):
    with open(self.location, "r") as file:
      reader = csv.reader(file)

      readData = [] # Form a list of all of the rows in the CSV
      for row in reader:
        readData.append(row)
    
    columns = readData[0] # The headers for each column
    readData.pop(0) # Prevent headers being read as a song

    columnLength = len(columns)
    for song in readData:
      tempSongData = {}
      if len(song) != columnLength: # Malformed CSV Row
        continue
      
      for count in range(0, columnLength): # Correct rows will have matching indexes for pairs
        tempSongData[columns[count]] = song[count]
      
      self.songData.append(tempSongData)


if __name__ == "__main__":
  if len(sys.argv) > 1:
    main(sys.argv[1])
  else:
    print("No location provided")