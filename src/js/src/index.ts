export function title_songname_detect(raw_title: string): string {
    let title: string;
    if (raw_title.includes("より【")) {
        title = raw_title.substring(
            raw_title.indexOf("【") + 1,
            raw_title.indexOf("】")
        );
    } else {
        title = raw_title;
    }
    title = title.replace(/^[\ud800-\ud8ff][\ud000-\udfff]{2,}/g, "");
    title = title.replace(/^[\ud000-\udfff]{2,}/g, "");
    if(title.startsWith("☔")){
        title = title.substring(1);
    }
    title = title.replace(
        /[<【（《〈\(\[].+?[】）》〉\)\]>]/g,
        ""
    );
    const keywords = ["主題歌", "OP", "CMソング", "オリジナル曲"];
    for (const keyword of keywords) {
        if (title.includes(`」${keyword}`)) {
            const end_index = title.indexOf(`」${keyword}`);
            for (let start_index = end_index; start_index >= 0; start_index--) {
                if (title[start_index] === "「") {
                    title =
                        title.substring(0, start_index) +
                        title.substring(end_index + keyword.length + 1);
                    break;
                }
            }
        }
    }
    for (const keyword of keywords) {
        if (title.includes(`』${keyword}`)) {
            const end_index = title.indexOf(`』${keyword}`);
            for (let start_index = end_index; start_index >= 0; start_index--) {
                if (title[start_index] === "『") {
                    title =
                        title.substring(0, start_index) +
                        title.substring(end_index + keyword.length + 1);
                    break;
                }
            }
        }
    }
    if (
        title.includes("「") &&
        title.includes("」")
    ) {
        const temp_title = title.substring(
            title.indexOf("「") + 1,
            title.indexOf("」")
        );
        let keyword_status = true;
        for (const keyword of keywords) {
            if (temp_title.includes(keyword)) {
                keyword_status = false;
                break;
            }
        }
        if (keyword_status && !temp_title.includes("cover")) {
            title = temp_title;
        }
    }
    if (
        title.includes("『") &&
        title.includes("』")
    ) {
        const temp_title = title.substring(
            title.indexOf("『") + 1,
            title.indexOf("』")
        );
        let keyword_status = true;
        for (const keyword of keywords) {
            if (temp_title.includes(keyword)) {
                keyword_status = false;
                break;
            }
        }
        if (keyword_status && !temp_title.includes("cover")) {
            title = temp_title;
        }
    }
    title = title.replace(/多重人格で歌ってみた.*/g, " ");
    title = title.replace(/コラボ歌ってみた.*/g, " ");
    title = title.replace(/歌って踊ってみた.*/g, " ");
    title = title.replace(/を歌ってみた.*/g, " ");
    title = title.replace(/歌ってみた.*/g, " ");
    title = title.replace(/[cC]over(ed)?( by)?.*/g, "");
    title = title.replace(/[fF]eat.*/g, "");
    title = title.replace(/描いて.*/g, "");
    title = title.replace(/喋って.*/g, "");
    if (!title.includes("ハロ/ハワユ")) {
        title = title.replace(/[/／].*/g, "");
    }
    title = title.replace(/\|.*/g, "");
    title = title.replace(/｜.*/g, "");
    title = title.replace(/[ 　]{2,}.*/g, "");
    title = title.replace(/[ 　]\//g, "");
    title = title.replace(/[Rr]emix.*/g, "");
    title = title.replace(/[?？]{3,}.*/g, "");
    title = title.replace(/[ 　]-.*/g, "");
    title = title.replace(/-[ 　].*/g, "");
    title = title.replace(/[ 　]*\.ver.*/g, "");
    title = title.replace(/／/g, "");
    title = title.replace(/#short/g, "");
    title = title.replace(/-/g, "");
    if (title.includes("MV") && title.substring(0, 2) === "MV") {
        title = title.substring(2);
    } else {
        title = title.replace(/MV.*/g, "");
    }
    title = title.replace(/music\s*video/gi, "");
    title = title.trim();
    if (title === "" && raw_title.length <= 10) {
        title = raw_title;
    }
    return title;
}

