#include "invertedindex.h"

InvertedIndex::InvertedIndex(istream& stream) {
    // читаем поток файлов в current_document
    for(string current_document; getline(stream, current_document); ){
        docs.push_back(move(current_document));
        // сохраняю идентификатор этого файла: нумерация по условию начинается с 0
        size_t docid = docs.size() - 1;
        /* файл теперь в базе и у нас есть его id - читаем из него текст,
         * с помощью SplitIntoWords - она читает последний документ в деке(как раз мы его
         * только что добавили) и возращает vector<string_view>
         * (почему не дек - вектор итерируется быстрее)
         * */
        for (string_view word : SplitIntoWords(docs.back()) ) {
            /* после того как SplitIntoWords вернула нам из документа вектор слов
             * запускаем цикл по обработке каждого слова
             * */
            /*
             * получаем ссылку на вектор значений для слова
             * */

            auto& docids = index[word];
            // если вектор Id/рейтинг не пустой и мы до сих пор в том де файле
            if (!docids.empty() && docids.back().docid==docid ) {
                docids.back().rating++;
            } else {
                docids.push_back({docid, 1});
            }
        }
    }
}

const vector<InvertedIndex::Entry>& InvertedIndex::Lookup(string_view word) const {
    // нахуя статик ???
    static vector<InvertedIndex::Entry> result;
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return result;
    }
}
