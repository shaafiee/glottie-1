
// functions

int stringToInt(string inputString) {
	stringstream streamer(inputString);
	int outputInt;
	streamer >> outputInt;
	return outputInt;
}

float stringToFloat(string inputString) {
	stringstream streamer(inputString);
	float outputFloat;
	streamer >> outputFloat;
	return outputFloat;
}

bool keyFound(struct KeyValue* tempKeyValue, string key) {
	if (tempKeyValue->key == key) {
		return true;
	}
	return false;
}

int addChildArray(struct KeyValue* traceKeyValue) {

	struct ArrayOfString* tempArrayOfString;
	tempArrayOfString = new ArrayOfString;
	if (traceKeyValue->arrayValue == NULL) {
		traceKeyValue->arrayValue = tempArrayOfString;
		return 1;
	}
	currentArrayOfString = tempArrayOfString;
	tempArrayOfString->parent = traceKeyValue->arrayValue;
	traceKeyValue->arrayValue->child = tempArrayOfString;
	traceKeyValue->arrayValue = tempArrayOfString;
	return 1;
}

int gotoParentArray(struct KeyValue* traceKeyValue) {
	if (traceKeyValue->arrayValue != NULL) {
		if (traceKeyValue->arrayValue->parent != NULL) {
			traceKeyValue->arrayValue = traceKeyValue->arrayValue->parent;
		}
	}
	return 1;
}

int popKeyValueTrail() {
	if (currentKeyValueTrail->prev == NULL) {
		if (currentKeyValueTrail != NULL) {
			delete currentKeyValueTrail;
			currentKeyValue = NULL;
			currentKeyValueTrail = NULL;
		}
		return 1;
	}
	currentKeyValueTrail = currentKeyValueTrail->prev;
	delete currentKeyValueTrail->next;
	currentKeyValueTrail->next = NULL;
	currentKeyValue = currentKeyValueTrail->keyValue;
	while (currentKeyValue->next != NULL) {
		currentKeyValue = currentKeyValue->next;
	}
	return 1;
}

int newKeyValueTrail() {
	if (currentKeyValueTrail == NULL) {
		currentKeyValueTrail = new KeyValueTrail;
		currentKeyValueTrail->start = currentKeyValueTrail;
		currentKeyValueTrail->next = NULL;
		currentKeyValueTrail->prev = NULL;
		EM_ASM({console.log(">>>>>>>>>>>>>>>>>>>> key trail created");});
		
		/* // since keyvalues are added on the fly
		struct KeyValue* tempKeyValue;
		tempKeyValue = new KeyValue;
		tempKeyValue->start = tempKeyValue;
		currentKeyValueTrail->keyValue = tempKeyValue;
		currentKeyValue = tempKeyValue;
		*/
	} else {
		struct KeyValueTrail* tempKeyValueTrail;
		tempKeyValueTrail = new KeyValueTrail;
		currentKeyValueTrail->next = tempKeyValueTrail;
		tempKeyValueTrail->prev = currentKeyValueTrail;
		tempKeyValueTrail->start = currentKeyValueTrail->start;
		currentKeyValueTrail = tempKeyValueTrail;
		EM_ASM({console.log(">>>>>>>>>>>>>>>>>>>> key trail extended");});
		
		/* // since keyvalues are added on the fly
		struct KeyValue* tempKeyValue;
		tempKeyValue = new KeyValue;
		tempKeyValue->start = tempKeyValue;
		currentKeyValueTrail->keyValue = tempKeyValue;
		currentKeyValue = tempKeyValue;
		*/
	}
	return 1;
}

int removeKeyValueTrail() { // to be called from within associateKeyValue()
	struct KeyValueTrail* tempKeyValueTrail;
	tempKeyValueTrail = currentKeyValueTrail;
	if (currentKeyValueTrail->prev != NULL) {
		currentKeyValueTrail = currentKeyValueTrail->prev;
		currentKeyValue = currentKeyValueTrail->keyValue;
		currentKeyValue->next = NULL;
		currentArrayOfString = currentKeyValueTrail->keyValue->arrayValue;
	} else {
		currentKeyValueTrail = new KeyValueTrail;
		currentKeyValueTrail->start = currentKeyValueTrail;
		currentKeyValueTrail->keyValue = new KeyValue;
		currentKeyValueTrail->keyValue->start = currentKeyValueTrail->keyValue;
		currentKeyValueTrail->keyValue->arrayValue = new ArrayOfString;
	}

	currentKeyValueTrail->next = NULL;
	delete tempKeyValueTrail->keyValue->arrayValue;
	delete tempKeyValueTrail->keyValue;
	delete tempKeyValueTrail;

	return 1;
}

int removeKeyValue() {
	struct KeyValue* tempKeyValue;
	currentKeyValue->prev->next = currentKeyValue->next;
	if (currentKeyValue->next != NULL) {
		currentKeyValue->next->prev = currentKeyValue->prev;
	}
	tempKeyValue = currentKeyValue->prev;
	delete currentKeyValue;
	currentKeyValue = tempKeyValue;

	return 1;
}

int pushValuesVector(struct ValuesVector* traceVector, string tempString) {
	struct ValuesVector* tempVector;
	tempVector = new ValuesVector;
	if (traceVector == NULL) {
		traceVector = new ValuesVector;
		traceVector->start = traceVector;
		traceVector->next = NULL;
		traceVector->prev = NULL;
		traceVector->value.reserve(30);
		traceVector->value = tempString;
	} else {
		traceVector->value.reserve(30);
		tempVector->value = tempString;
		traceVector->next = tempVector;
		tempVector->prev = traceVector;
		tempVector->start = traceVector->start;
		traceVector = tempVector;
	}
	return 1;
}

string popValuesVector(struct ValuesVector* traceVector) {
	if (traceVector != NULL) {
		struct ValuesVector* tempVector;
		tempVector = traceVector;
		string tempString = tempVector->value;
		if (traceVector->prev != NULL) {
			traceVector = traceVector->prev;
			traceVector->next = NULL;
			return tempString;
		} else {
			return tempString;
		}
		delete tempVector;
	}
	return NULL;
}

int addKeyValue(struct KeyValue* traceKeyValue, string key, string value, bool isArray) {
	bool exhausted = false;
	struct KeyValue* keyNode = NULL;
	keyNode = new KeyValue;
	struct KeyValue* endNode = NULL;
	endNode = new KeyValue;
	if (traceKeyValue != NULL) {
		struct KeyValue* tempKeyValue = traceKeyValue;
		if (keyFound(traceKeyValue, key)) {
			keyNode = traceKeyValue;
		} else {
			while (! exhausted) {
				if (keyFound(tempKeyValue, key)) {
					keyNode = tempKeyValue;
				}
				if (tempKeyValue->next == NULL) {
					endNode = tempKeyValue;
					exhausted = true;
				}
				tempKeyValue = tempKeyValue->next;
			}
		}
	} else {
		traceKeyValue = new KeyValue;
		traceKeyValue->start = traceKeyValue;
		traceKeyValue->prev = NULL;
		traceKeyValue->next = NULL;
		keyNode = traceKeyValue;
		keyNode->arrayValue = new ArrayOfString;
		keyNode->key = key;
	}
	if (keyNode == NULL) {
		endNode->next = keyNode;
		keyNode->prev = endNode;
		keyNode->start = traceKeyValue->start;
		keyNode->arrayValue = new ArrayOfString;
		keyNode->key = key;
	}
	if (isArray) {
		/*
			struct ArrayOfString* tempArrayOfString;
			tempArrayOfString = new ArrayOfString;
			currentArrayOfString = tempArrayOfString;
			keyNode->arrayValue->child = tempArrayOfString;
			tempArrayOfString->parent = keyNode->arrayValue;
			keyNode->arrayValue = tempArrayOfString;
		*/
		//keyNode->arrayValue->value.push_back(value);
		pushValuesVector(keyNode->arrayValue->vector, value);
	} else {
		keyNode->value = value;
	}
	EM_ASM_({console.log(">>>>>>>>>>>>>>>>>>>> key added " + String.fromCharCode($0));}, keyNode->key.at(0));
	if (!value.empty()) {
		EM_ASM_({console.log(">>>>>>>>>>>>>>>>>>>> with value " + String.fromCharCode($0));}, value.at(0));
	}
	traceKeyValue = keyNode;
	return 1;
}

int pushVertex(struct ArrayOfVertex* passedVertex, float vertex[4]) {
	struct ArrayOfVertex* tempAOV;
	tempAOV = new ArrayOfVertex;
	if (passedVertex == NULL) {
		passedVertex = tempAOV;
		passedVertex->start = tempAOV;
		passedVertex->prev = NULL;
		passedVertex->next = NULL;
	} else {
		bool exhausted = false;
		while (! exhausted) {
			if (passedVertex->next == NULL) {
				exhausted = true;
			} else {
				passedVertex = passedVertex->next;
			}
		}
		tempAOV->prev = passedVertex;
		passedVertex->next = tempAOV;
		tempAOV->start = passedVertex->start;
		passedVertex = tempAOV;
	}

	passedVertex->vertex->position[0] = vertex[0];
	passedVertex->vertex->position[1] = vertex[1];
	passedVertex->vertex->position[2] = vertex[2];
	passedVertex->vertex->position[3] = vertex[3];

	return 1;
}

//////////// type converters


