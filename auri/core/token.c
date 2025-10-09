#include "auri/core/token.h"

#include "auri/utils/string.h"

#include <stdio.h>

void auri_token_free(AuriToken* token) {
    auri_strfree(&token->lexeme);
    auri_strfree(&token->literal);
}