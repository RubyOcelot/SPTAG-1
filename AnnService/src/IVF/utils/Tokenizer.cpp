#include "inc/IVF/utils/Tokenizer.h"

namespace IVF{

    Tokenizer::Tokenizer(std::unique_ptr<std::istream> inputStream):inputStream(std::move(inputStream)) {

    }
}