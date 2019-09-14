#include "ini.h"
#include <iostream>
#include "test_runner.h"

// без std::move

using namespace std;
using namespace Ini;


Section& Ini::Document::AddSection(std::string name) {
      return sections[name];
}
const Section& Ini::Document::GetSection(const std::string& name) const {
      return sections.at(name);
}
std::size_t Ini::Document::SectionCount() const {
      return sections.size();
}
Document Ini::Load(std::istream& input) {
            Document doc;
            std::string line, name_section;
            while (std::getline(input, line)) {
                  if (line.empty()) {
                        continue;
                  }
                  if (line.front()=='[') {
                        name_section = line.substr(1, line.size() - 2);
                        // случай если добавляют пустую секцию
                        doc.AddSection(name_section);
                  }
                  else {
                        std::size_t index = line.find('=');
                        std::string key = line.substr(0, index);
                        std::string value = line.substr(index+1, std::string::npos);
                        doc.AddSection(name_section).insert( {key, value} );
                  }
            }
            return doc;
      }
