import csv
import unicodedata
#from ...src.python.titleSongnameDetect import get_song_title

correct_count = 0
total_count = 0

with open("../testdata.csv",encoding="utf-8") as f:
    reader = csv.reader(f)
    for row in reader:
        title = row[1]
        songname = row[2]
        songname = unicodedata.normalize('NFKC', songname)
        detect_musicname = get_song_title(title)
        detect_musicname = unicodedata.normalize('NFKC', detect_musicname)
        if detect_musicname == songname:
            correct_count += 1
        else:
            print("title: {}, detect_musicname: {}".format(title, detect_musicname))
        total_count += 1

print("correct_count: {}, total_count: {}, correct rate:{}".format(correct_count, total_count, (correct_count/total_count)*100))
