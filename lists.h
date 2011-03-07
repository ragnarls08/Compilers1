#include "token.h"


TokenCode pProgramDefinition[6] = { tc_SEMICOL, tc_VAR, tc_FUNCTION, tc_PROCEDURE, tc_BEGIN, (TokenCode)'\0' };

TokenCode pDeclarations[1] = {(TokenCode)'\0'};
TokenCode pSubprogramDeclarations[1] = {(TokenCode)'\0'};
TokenCode pCompoundStatement[1] = {(TokenCode)'\0'};


TokenCode pIdentifierList[2] = { tc_RPAREN, (TokenCode)'\0'};
TokenCode pIdentifierListMore[1] = {(TokenCode)'\0'};

TokenCode pParameterListMore[1] = {(TokenCode)'\0'};
TokenCode pIdentifierListAndType[1] = {(TokenCode)'\0'};
TokenCode pSubprogramHead[1] = {(TokenCode)'\0'};
TokenCode pSubprogramDeclaration[1] = {(TokenCode)'\0'};


TokenCode pFactor[1] = {(TokenCode)'\0'};
TokenCode pFactorRest[1] = {(TokenCode)'\0'};
TokenCode pTerm[1] = {(TokenCode)'\0'};
TokenCode pTermRest[1] = {(TokenCode)'\0'};

TokenCode pExpression[1] = {(TokenCode)'\0'};
TokenCode pSimpleExpression[1] = {(TokenCode)'\0'};
TokenCode pExpressionList[1] = {(TokenCode)'\0'};
TokenCode pExpressionAddop[1] = {(TokenCode)'\0'};
TokenCode pArrayReference[1] = {(TokenCode)'\0'};
TokenCode pExpressionListMore[1] = {(TokenCode)'\0'};

TokenCode pSimpleExpressionAddop[1] = {(TokenCode)'\0'};
TokenCode pIdOrProcedureStatement[1] = {(TokenCode)'\0'};
TokenCode pIfStatement[1] = {(TokenCode)'\0'};
TokenCode pWhileStatement[1] = {(TokenCode)'\0'};
TokenCode pStatementListMore[1] = {(TokenCode)'\0'};
TokenCode pStatement[1] = {(TokenCode)'\0'};
TokenCode pType[1] = {(TokenCode)'\0'};

TokenCode pStandardType[1] = {(TokenCode)'\0'};
TokenCode pArguments[1] = {(TokenCode)'\0'};

TokenCode pParameterList[1] = {(TokenCode)'\0'};
TokenCode pOptionalStatement[1] = {(TokenCode)'\0'};
TokenCode pStatementList[1] = {(TokenCode)'\0'};
TokenCode pSimpleExpressionRelop[1] = {(TokenCode)'\0'};






