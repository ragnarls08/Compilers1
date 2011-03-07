#include "token.h"
// follow sets

TokenCode pProgramDefinition[6] = { tc_SEMICOL, (TokenCode)'\0' };

TokenCode pDeclarations[1] = { tc_FUNCTION, tc_PROCEDURE, tc_BEGIN, (TokenCode)'\0'};

TokenCode pIdentifierList[2] = { tc_RPAREN, tc_COLON, (TokenCode)'\0'};
TokenCode pIdentifierListMore[1] = {tc_RPAREN, tc_COLON, (TokenCode)'\0'};

TokenCode pIdentifierListAndType[1] = {tc_SEMICOL, tc_RPAREN, (TokenCode)'\0'};
TokenCode pType[1] = {tc_SEMICOL, tc_RPAREN, (TokenCode)'\0'};

TokenCode pStandardType[1] = {tc_SEMICOL, tc_RPAREN, (TokenCode)'\0'};
TokenCode pSubprogramDeclarations[1] = {tc_BEGIN, (TokenCode)'\0'};
TokenCode pSubprogramDeclaration[1] = {tc_SEMICOL, (TokenCode)'\0'};
TokenCode pSubprogramHead[1] = {tc_BEGIN, tc_VAR, (TokenCode)'\0'};

TokenCode pArguments[1] = {tc_COLON, tc_SEMICOL, (TokenCode)'\0'};
TokenCode pParameterList[1] = {tc_RPAREN, (TokenCode)'\0'};
TokenCode pParameterListMore[1] = {tc_RPAREN, (TokenCode)'\0'};

TokenCode pCompoundStatement[1] = {tc_DOT, tc_SEMICOL, (TokenCode)'\0'};

TokenCode pOptionalStatement[1] = {tc_END, (TokenCode)'\0'};
TokenCode pStatementList[1] = {tc_END, (TokenCode)'\0'};
TokenCode pStatementListMore[1] = {tc_END, (TokenCode)'\0'};
TokenCode pStatement[1] = {tc_END, tc_SEMICOL, tc_ELSE (TokenCode)'\0'};

TokenCode pArrayReference[1] = {tc_ASSIGNOP, tc_MULOP, tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pIdOrProcedureStatement[1] = {tc_END, tc_SEMICOL, tc_ELSE, (TokenCode)'\0'};

TokenCode pExpressionList[1] = {tc_RPAREN, (TokenCode)'\0'};
TokenCode pExpressionListMore[1] = {tc_RPAREN, (TokenCode)'\0'};
TokenCode pExpression[1] = {tc_RPAREN, tc_COMMA, tc_ELSE, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, (TokenCode)'\0'};

TokenCode pSimpleExpressionRelop[1] = {tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_ELSE, (TokenCode)'\0'};
TokenCode pSimpleExpression[1] = {tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pSimpleExpressionAddop[1] = {tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP,(TokenCode)'\0'};



TokenCode pTerm[1] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pTermRest[1] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};


TokenCode pFactor[1] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};
TokenCode pFactorRest[1] = {tc_ADDOP, tc_RPAREN, tc_COMMA, tc_END, tc_SEMICOL, tc_RBRACKET, tc_DO, tc_THEN, tc_RELOP, tc_ELSE, (TokenCode)'\0'};





TokenCode pIfStatement[1] = {(TokenCode)'\0'};
TokenCode pWhileStatement[1] = {(TokenCode)'\0'};



//TokenCode pExpressionAddop[1] = {(TokenCode)'\0'};







