import * as fs from 'fs';
import { parse } from 'csv-parse/sync';
import * as tsd from '../../../src/js/src/index.js';

const data = fs.readFileSync('testdata.csv');
const records = parse(data);
let correct_count = 0;
const total_count = records.length;
for (const record of records) {
    const title = record[1];
    const correct = record[2];
    const correct_normalized = correct.normalize('NFKC');
    const result = tsd.title_songname_detect(title);
    const result_normalized = result.normalize('NFKC');
    if(result_normalized === correct_normalized) {
        correct_count++;
    }
    else{
        console.log(`title: ${title}\tdetected: ${result_normalized}`);
    }
}
console.log(`correct: ${correct_count}\ntotal: ${total_count}\naccuracy: ${(correct_count / total_count)*100}%`);
