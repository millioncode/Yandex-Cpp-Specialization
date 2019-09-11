#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>

#include <deque>
#include <string_view>
using namespace std;
vector<string_view> SplitIntoWords(string_view line);
class InvertedIndex {
    public:
        // создам конструктор - принимает поток файлов
        InvertedIndex() = default ;
        explicit InvertedIndex(istream& is);

        //void Add(const string& document);
        // заменю лист на дек
        struct Entry {
                size_t docid;
                size_t rating;
        };
        const vector<Entry>& Lookup(string_view word) const ;

        const deque<string>& GetDocument() const {
            return docs;
        }

    private:
        /* почему эти контейнеры
         * повтор слов не нужен
         * каждому слову соответствует вектор [ id файла и рейтинг в этом файле у этого слова]
         * */
        map <string_view, vector<Entry> > index;
        // дек док-ов, нужно добавлять и обращаться к последнему
        deque<string> docs;
};

#endif // INVERTEDINDEX_H
