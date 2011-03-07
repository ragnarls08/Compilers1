#include "token.h"
// follow sets

TokenCode pProgramDefinition[2] = { tc_SEMICOL, (TokenCode)'\0' };

TokenCode pDeclarations[4] = { tc_FUNCTION, tc_PROCEDURE, tc_BEGIN, (TokenCode)'\0'};

TokenCode pIdentifierList[3] = { tc_RPAREN, tc_COLON, (TokenCode)'\0'};
TokenCode pIdentifierListMore[3] = {tc_RPAREN, tc_COLON, (TokenCode)'\0'};

TokenCode pIdentifierListAndType[3] = {tc_SEMICOL, tc_RPAREN, (TokenCode)'\0'};
TokenCode pType[3] = {tc_SEMICOL, tc_RPAREN, (TokenCode)'\0'};

TokenCode pStandardType[3] = {tc_SEMICOL, tc_RPAREN, (TokenCode)'\0'};
TokenCode pSubprogramDeclarations[2] = {tc_BEGIN, (TokenCode)'\0'};
TokenCode pSubprogramDeclaration[2] = {tc_SEMICOL, (TokenCode)'\0'};
TokenCode pSubprogramHead[3] = {tc_BEGIN, tc_VAR, (TokenCode)'\0'};

TokenCode pArguments[3] = {tc_COLON, tc_SEMICOL, (TokenCode)'\0'};
TokenCode pParameterList[2] = {tc_RPAREN, (TokenCode)'\0'};
TokenCode pParameterListMore[2] = {tc_RPAREN, (TokenCode)'\0'};

TokenCode pCompoundStatement[3] = {tc_DOT, tc_SEMICOL, (TokenCode)'\0'};

TokenCode pOptionalStatement[2] = {tc_END, (TokenCode)'\0'};
TokenCode pStatementList[2] = {tc_END, (TokenCode)'\0'};
TokenCode pStatementListMore[2] = {tc_END, (TokenCode)'\0'};
TokenCode pStatement[4] = {tc_END, tc_SEMICOL, tc_ELSE, (TokenCode)'\0'};

TokenCode pArrayReference[13] = {tc_ASSIGNOP, tc_MULOP, tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pIdOrProcedureStatement[4] = {tc_END, tc_SEMICOL, tc_ELSE, (TokenCode)'\0'};

TokenCode pExpressionList[2] = {tc_RPAREN, (TokenCode)'\0'};
TokenCode pExpressionListMore[2] = {tc_RPAREN, (TokenCode)'\0'};
TokenCode pExpression[9] = {tc_RPAREN, tc_COMMA, tc_ELSE, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, (TokenCode)'\0'};

TokenCode pSimpleExpressionRelop[9] = {tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_ELSE, (TokenCode)'\0'};
TokenCode pSimpleExpression[10] = {tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pSimpleExpressionAddop[9] = {tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP,(TokenCode)'\0'};


TokenCode pTerm[11] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pTermRest[11] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};


TokenCode pFactor[11] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pFactorRest[11] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};


TokenCode pIfStatement[4] = {tc_END, tc_SEMICOL, tc_ELSE, (TokenCode)'\0'};
TokenCode pWhileStatement[4] = {tc_END, tc_SEMICOL, tc_ELSE, (TokenCode)'\0'};



//TokenCode pExpressionAddop[1] = {(TokenCode)'\0'};







