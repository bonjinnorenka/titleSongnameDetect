import re

def get_song_title(raw_title):

    # 「作品名」より【楽曲タイトル】 というパターンがあるので、その場合は【】の中身をタイトルとする
    if "より【" in raw_title:
        title = raw_title.split("【")[1].split("】")[0]
    else:
        title = raw_title

    # ヘッダー的に記号がついていたら削除する
    if title[0] == "★":
        title = title[1:]

    # ()（）[]【】を除外する。左が半角で右が全角だったりすることもある
    title = re.sub("[<【（《\(\[].+?[】）》〉\)\]>]"," ",title)

    # 「作品名」主題歌 などのパターンの場合は、その部分を消す
    for keyword in ["主題歌", "OP", "CMソング","オリジナル曲"]:
        if "」{}".format(keyword) in title:
            end_index = title.index("」{}".format(keyword))
            for start_index in range(end_index, -1, -1):
                if title[start_index] == "「":
                    title = title[:start_index] + title[end_index + len(keyword) + 1:]
                    break

    for keyword in ["主題歌", "OP", "CMソング","オリジナル曲"]:
        if "』{}".format(keyword) in title:
            end_index = title.index("』{}".format(keyword))
            for start_index in range(end_index, -1, -1):
                if title[start_index] == "『":
                    title = title[:start_index] + title[end_index + len(keyword) + 1:]
                    break

    # 「」と『』がある場合、その中の文字列を取り出す
    # ただし、稀に「」の中に自分の名前を入れている場合がある。その場合は無視する
    if "「" in title and "」" in title:
        temp_title = title = title.split("「")[1].split("」")[0]
        if "cover" not in temp_title.lower():
            title = temp_title

    if "『" in title and "』" in title:
        temp_title = title.split("『")[1].split("』")[0]
        if "cover" not in temp_title.lower():
            title = temp_title

    # 歌ってみた以降の文字列を消す
    title = re.sub("を歌ってみた.*"," ", title)
    title = re.sub("歌ってみた.*"," ", title)

    title = re.sub("描いて.*"," ", title)

    # cover, covered, covered by 以降の文字列を消す
    title = re.sub("[cC]over(ed)?( by)?.*", "", title)
    title = re.sub("[Ff]eat.*", "", title)

    title = re.sub("[Mm]usic[ 　s][Vv]ideo","",title)

    # /以降は削除する
    if "/" in title:
        title = title.split("/")[0]

    if "／" in title:
        title =  title.split("／")[0]

    # - があったらその後ろを消す
    title = title.split("-")[0]

    # コラボメンバーを×で表現している部分を消す
    # #012 的な表現を消す
    title_part_list = []
    for title_part in title.split(" "):
        if "×" not in title_part and not re.fullmatch("#[0-9]+", title_part):
            title_part_list.append(title_part)
    title = " ".join(title_part_list)

    if(title.find("MV")==0):
        title = title[2:]
    else:
        title = re.sub("MV.*","",title)

    # 前後の空白を削除
    title = title.strip()

    return title
