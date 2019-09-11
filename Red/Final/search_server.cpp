#include "search_server.h"
#include "iterator_range.h"
#include "new_profile.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>
#include <vector>


void SearchServer::UpdateDocumentBase(istream& document_input) {
    index = InvertedIndex{document_input};
}

void SearchServer::AddQueriesStream(
        istream& query_input, ostream& search_results_output
        ) {
    // ссылка на дек документов(стринг)
    const auto& documents = index.GetDocument();
    // номер ячейки - Id файла, а значение - рейтинг
    vector<size_t> docid_count(documents.size());
    vector <int64_t> docids(documents.size());
    // в потоке query_input - слова, которые мы ищем по файлам

    TotalDuration words_split("  Words split");
    TotalDuration lookup("  Lookup");
    TotalDuration sorting("  Sort");
    TotalDuration build_results("  Build results");
    TotalDuration total_iteration("  Total loop iteration");
    for (string current_query; getline(query_input, current_query); ) {
        ADD_DURATION(total_iteration);
        vector<string_view> words;
        // SplitIntoWords возращает vector<string_view>
        {
            ADD_DURATION(words_split);
            words = SplitIntoWords(current_query);
        }
        docid_count.assign(docid_count.size(), 0);
        {
            ADD_DURATION(lookup);
            for (const auto& word : words) {
                // спасибо С++17
                for (const auto& [docid, rating]: index.Lookup(word)) {
                    // по сути копируем рейтинг в файлах в docid_count
                    docid_count[docid]+= rating;
                }
            }
        }
        // заполняем от 0 до documents.size()
        iota(docids.begin(), docids.end(), 0);
        // сортируем id документов смотря при этом значения в docid_count
        // т.к. по условию нам нужно только 5 значений - вместо sort пользую partial_sort
        {
            ADD_DURATION(sorting);
            partial_sort(
                        begin(docids),
                        Head(docids, 5).end(),
                        end(docids),
                        [&docid_count](int64_t lhs, int64_t rhs) {
                return make_pair(docid_count[lhs], -lhs) > make_pair(docid_count[rhs], -rhs);
            }
            );
        }
        // Выводим результат
        // исходный запрос
        ADD_DURATION(build_results);
        search_results_output << current_query << ':';
                for (size_t docid : Head(docids, 5)) {
                    // рейтинг
                  const size_t hit_count = docid_count[docid];
                  // если рейтинг==0, то не будем добавлять в поток
                  if (hit_count == 0) {
                      break;
                  }
                  search_results_output << " {"
                  << "docid: " << docid << ", "
                  << "hitcount: " << hit_count << '}';
                }
                search_results_output << '\n';
    }
}

