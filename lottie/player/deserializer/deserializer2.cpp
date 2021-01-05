


/////////////////////////////////////////////////////////////////////////////

int resetCurrentValue() {
	input->currentValueLength = 0;
	input->currentValue[0] = '\0';

	return 1;
}

int assignReadValue() {
#ifdef DEBUG
#ifdef EMT
#else
	cout << "reset currentReadValue\n";
#endif
#endif
	input->currentReadValue[0] = '\0';
#ifdef DEBUG
#ifdef EMT
#else
	cout << "copy to currentReadValue\n";
#endif
#endif

	if (input->currentValueLength > 0) {
		strcat(input->currentReadValue, input->currentValue);
	}
	else {
		input->currentReadValue[0] = '\0';
	}
#ifdef DEBUG
#ifdef EMT
#else
	cout << "reset currentValue\n";
#endif
#endif
	if (readingArray) {
#ifdef DEBUG2
		//cout << input->currentReadValue << "\n";
#endif
	}
	//resetCurrentValue();
	return 1;
}

int assignReadKey() {
#ifdef DEBUG
#ifdef EMT
#else
	cout << "reset currentReadKey\n";
#endif
#endif
	input->currentReadKey[0] = '\0';
#ifdef DEBUG
#ifdef EMT
#else
	cout << "copy to currentReadKey\n";
#endif
#endif
	if (input->currentValueLength > 0) {
		strcpy(input->currentReadKey, input->currentValue);
	}
	else {
#ifdef DEBUG
#ifdef EMT
#else
		cout << "empty key\n";
#endif
#endif
		input->currentReadKey[0] = '\0';
	}
#ifdef DEBUG
#ifdef EMT
#else
	cout << "reset currentValue\n";
#endif
#endif
	//resetCurrentValue();
	return 1;
}


int appendCurrentValue(char passedChar) {
	if (input->currentValueLength >= KVLEN - 1) {
		return 0;
	}
	input->currentValue[input->currentValueLength] = passedChar;
	input->currentValue[input->currentValueLength + 1] = '\0';
	input->currentValueLength = input->currentValueLength + 1;
	return 1;
}

int addState(enum States statePassed) {
	if (theState->stateNow == KVReading && statePassed == KVReading) {
		return 0;
	}
	struct StateTrail* tempState = NULL;
	delete tempState;
	tempState = new StateTrail;
	tempState->prev = theState;

	theState->next = tempState;
	tempState->start = theState->start;
	tempState->stateNow = statePassed;
	theState = tempState;

	return 1;
}

int removeState() {
	if (theState != NULL && theState->prev != NULL) {
		struct StateTrail* tempState;
		tempState = theState;
		theState = theState->prev;
		theState->next = NULL;
		//EM_ASM_({console.log("removed state 700.1 " + $0);}, theState);
		delete tempState;
		tempState = NULL;
		//EM_ASM_({console.log("removed state 700.2 " + $0);}, theState);
	}
	else {
		if (theState != NULL) {
			delete theState;
		}
		theState = NULL;
		theState = new StateTrail;
		theState->start = theState;
		theState->stateNow = NoState;
	}

	return 1;
}

int removeReadStates() {
	struct StateTrail* tempState;
	while (theState->stateNow == KVReading || theState->stateNow == KVReadOpen) {
		//EM_ASM_({console.log("looping " + $0);}, theState->stateNow);

		if (theState->prev == NULL) {
			theState->next = NULL;
			theState->stateNow = NoState;
			break;
		}

		tempState = theState;
		theState = theState->prev;
		theState->next = NULL;
		delete tempState;
	}
	/*if (theState->prev == NULL && (theState->stateNow == KVReading || theState->stateNow == KVReadOpen)) {
		delete theState;
		theState = new StateTrail;
		theState->start = theState;
	}*/
	if (theState->stateNow == KVReading || theState->stateNow == KVReadOpen) {
		removeState();
	}

	//EM_ASM_({console.log("remove read states done 1.5 " + $0);}, theState->stateNow);


	return 1;
}

int addScope(enum Scopes scopePassed) {

	struct ScopeTrail* tempScope;
	if (scopePassed == NULL) {
		tempScope = new ScopeTrail;
		tempScope->start = tempScope;
		theScope = tempScope;
		theScope->currentState = theState;
	}
	else {
		tempScope = new ScopeTrail;
		tempScope->prev = theScope;
		tempScope->next = NULL;
		theScope->next = tempScope;
		tempScope->start = theScope->start;
		theScope = tempScope;
		theScope->currentState = theState;
	}

	/*
	struct ScopeTrail* tempScope;
	tempScope = new ScopeTrail;
	theScope->next = tempScope;
	tempScope->prev = theScope;
	theScope = tempScope;
	theScope->next = NULL;
	theScope->currentState = theState;
	theScope->start = theScope->prev->start;
	*/

	theScope->scope = scopePassed;
	//EM_ASM_({console.log("scope successfully added " + $0);}, theState->stateNow);

	return 1;
}

struct ScopeTrail* removeObjectsFromScope(struct ScopeTrail* passedScope) {
	struct ScopeTrail* tempScope;
	while (passedScope->prev != NULL && passedScope->scope == _object) {
		tempScope = passedScope;
		passedScope = passedScope->prev;
		passedScope->next = NULL;
		delete tempScope;
	}
	return passedScope;
}

int removeScope() {
	//EM_ASM_({console.log($0);}, (int)theScope->scope);

	struct ScopeTrail* tempScope;

	if (theScope == NULL) {
		addScope(_noscope);
		addScope(_animation);
		return 1;
	}

	//EM_ASM({console.log("removing scope 1.1 ");});
	tempScope = theScope;

	//EM_ASM({console.log("removing scope 1.2 ");});
	if (theScope->prev != NULL) {
		//EM_ASM({console.log("removing scope 1.1 ");});
		if (theScope->currentKeyValueTrail != NULL) {
			//EM_ASM({console.log("removing scope 1.1.0 ");});
			//deleteKeyValues(theScope->currentKeyValueTrail);
			//EM_ASM({console.log("removing scope 1.1.1 ");});
			//delete theScope->currentKeyValueTrail;
			theScope->currentKeyValueTrail = NULL;
			currentKeyValueTrail = theScope->currentKeyValueTrail;
			/*
			if (theScope->currentKeyValueTrail != NULL && theScope->currentKeyValueTrail->keyValue != NULL) {
				currentKeyValue = theScope->currentKeyValueTrail->keyValue;
				//EM_ASM({console.log("removing scope 1.1.3 ");});
				if (theScope->currentKeyValueTrail->keyValue->arrayValue != NULL) {
					currentArrayOfString = theScope->currentKeyValueTrail->keyValue->arrayValue;
					//EM_ASM({console.log("removing scope 1.1.4 ");});
				}
			}
			*/
		}
		//EM_ASM({console.log("removing scope 1.3 ");});
		theScope = theScope->prev;
		//EM_ASM({console.log("removing scope 1.3a ");});
		tempScope->prev = NULL;
		//EM_ASM({console.log("removing scope 1.3b ");});
		theScope->next = NULL;
		currentKeyValueTrail = theScope->currentKeyValueTrail;
		//EM_ASM({console.log("removing scope 1.3c ");});
		delete tempScope;
		//EM_ASM({console.log("removing scope 1.3d ");});
		tempScope = NULL;
	}
	else {
		theScope->next = NULL;
		//deleteKeyValues(theScope->currentKeyValueTrail);
		//delete theScope->currentKeyValueTrail;
		theScope->currentKeyValueTrail = NULL;
	}
	//EM_ASM_({console.log("left behind " + $0);}, theScope->scope);

	return 1;
}

int addArray() {
	struct ArrayTrail* tempArray;
	tempArray = new ArrayTrail;
	arrayNow->next = tempArray;
	tempArray->prev = arrayNow;
	arrayNow = arrayNow->next;
	arrayNow->start = arrayNow->prev->start;
	arrayNow->next = NULL;

	return 1;
}

int removeArray() {
	struct ArrayTrail* tempArray;
	tempArray = arrayNow;
	tempArray->prev = NULL;
	arrayNow = arrayNow->prev;
	arrayNow->next = NULL;
	delete tempArray;

	return 1;
}

#include "properties.cpp"
#include "layers.cpp"
#include "shapes.cpp"
#include "helpers.cpp"
#include "associate.cpp"

struct ScopeTrail* lastScopeBeforeThis(struct ScopeTrail* tempScopeTrail) {
	if (tempScopeTrail == NULL) {
		return NULL;
	}
	int counter = 0;
	while (tempScopeTrail->prev != NULL && tempScopeTrail->scope == _object) {
		counter = counter + 1;
		tempScopeTrail = tempScopeTrail->prev;
	}
	return tempScopeTrail;
}

struct ScopeBefore lastScopeBeforeObject() {
	struct ScopeBefore result;
	struct ScopeTrail* tempScopeTrail;
	tempScopeTrail = theScope;
	long counter = 0;
	while (tempScopeTrail->prev != NULL && tempScopeTrail->scope == _object) {
		counter = counter + 1;
		tempScopeTrail = tempScopeTrail->prev;
	}
	result.scopeNow = tempScopeTrail;
	result.objectCount = counter;
	return result;
}

bool keyIs(char const* passedKey) {
	if (strcmp(input->currentReadKey, passedKey) == 0) {
		return true;
	}
	else {
		return false;
	}
}

int checkScope() {
	struct ScopeBefore previousScope = lastScopeBeforeObject();
	scopeChanged = true;
	if (previousScope.scopeNow->scope == _noscope) {
		addScope(_animation);
	}
	else if (keyIs("assets")) {
		//EM_ASM({console.log("-/--> found assets");});
		addScope(_assets);
	}
	else if (keyIs("layers")) {
		//EM_ASM({console.log("-/--> found layers");});
		addScope(_layers);
	}
	else if (keyIs("shapes")) {
		//EM_ASM({console.log("-/--> found shapes");});
		addScope(_shapes);
	}
	else if (keyIs("it")) {
		//EM_ASM({console.log("-/--> found it");});
		addScope(_it);
	}
	else if (keyIs("ty")) {
		addScope(_ty);
	}
	else if (keyIs("ks")) {
		//EM_ASM({console.log("-/--> found ks");});
		addScope(_ks);
	}
	else if (keyIs("k")) {
		//EM_ASM({console.log("-/--> found k");});
		addScope(_k);
	}
	else if (keyIs("e")) {
		addScope(_e);
	}
	else if (keyIs("s")) {
		addScope(_s);
	}
	else if (keyIs("a")) {
		addScope(_a);
	}
	else if (keyIs("p")) {
		addScope(_p);
	}
	else if (keyIs("r")) {
		addScope(_r);
	}
	else if (keyIs("o")) {
		addScope(_o);
	}
	else if (keyIs("sk")) {
		addScope(_sk);
	}
	else if (keyIs("sa")) {
		addScope(_sa);
	}
	else if (keyIs("c")) {
		//EM_ASM({console.log("-/--> found c");});
		addScope(_c);
	}
	else if (keyIs("px")) {
		addScope(_px);
	}
	else if (keyIs("py")) {
		addScope(_py);
	}
	else if (keyIs("pz")) {
		addScope(_pz);
	}
	else if (keyIs("i")) {
		addScope(_i);
		//EM_ASM({console.log("-/--> found i");});
	}
	else {
		addScope(_object);
		scopeChanged = false;
	}

	//EM_ASM({console.log("-/--> NEW SCOPE " + $0);}, theScope->currentState->stateNow);

	return 1;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int readingDone() {
	struct KeyValue* tempKeyValue;
	if (readingArray) {
		tempKeyValue = addKeyValue(theScope->currentKeyValueTrail->keyValue, input->currentReadKey, input->currentReadValue, true);
		cout << "Added value " << input->currentReadValue << " to key " << input->currentReadKey << "\n";
	}
	else {
		tempKeyValue = addKeyValue(theScope->currentKeyValueTrail->keyValue, input->currentReadKey, input->currentReadValue, false);
	}
	theScope->currentKeyValueTrail->keyValue = tempKeyValue;
	return 1;
}

bool isReadingDone() {
	//EM_ASM({console.log("reading 100.1");});
	if ((theState->stateNow == KVReading || theState->stateNow == KVReadOpen) || strlen(input->currentValue) > 0) {
		//EM_ASM({console.log("reading 100.2");});
		if (kvState == Value) {
			//currentReadValue = input->currentValue;
#ifdef DEBUG
#ifdef EMT
#else
			cout << "assigning read values\n";
#endif
#endif
			assignReadValue();
			lastTypeEncountered[0] = '\0';
#ifdef DEBUG
#ifdef EMT
#else
			cout << "strcat\n";
#endif
#endif
			strcat(lastTypeEncountered, input->currentReadValue);
			//EM_ASM({console.log("reading 100.2.1");});
#ifdef DEBUG
#ifdef EMT
#else
			cout << "reading done\n";
#endif
#endif
			readingDone();
			//EM_ASM({console.log("reading 100.2.2");});
			//currentValue.clear();
			//input->currentValue[0] = '\0';
#ifdef DEBUG
#ifdef EMT
#else
			cout << "reset current value\n";
#endif
#endif
			resetCurrentValue();
			//EM_ASM({console.log("reading 100.3");});
		} else {
			//currentReadKey = input->currentValue;
#ifdef DEBUG
#ifdef EMT
#else
			cout << "assign read key\n";
#endif
#endif
			assignReadKey();
			//EM_ASM_({console.log($0);}, (int)theScope->scope);
			//currentValue.clear();
			//input->currentValue[0] = '\0';
#ifdef DEBUG
#ifdef EMT
#else
			cout << "reset current value\n";
#endif
#endif
			resetCurrentValue();
		}
		//EM_ASM_({console.log("post-reading " + $0);}, theState->stateNow);
		return true;
	}
	else {
		//currentValue.clear();
	}
	//EM_ASM({console.log("reading 100.9");});
	return false;
}

enum States lastStateBeforeReading() {
	struct StateTrail* tempState;
	tempState = theState;
	if (tempState == NULL) {
		return NoState;
	}

	if (tempState->stateNow != KVReadOpen && tempState->stateNow != KVReading) {
		return tempState->stateNow;
	}
	while (tempState->prev != NULL && (tempState->stateNow == KVReadOpen || tempState->stateNow == KVReading)) {
		tempState = tempState->prev;
	}
	return tempState->stateNow;
}

int checkCharacter(char currentChar) {
	//EM_ASM_({console.log("YAC +++++++++++++++++++++++++++++++++++++++> " + " [ " + $1 + " ] " + String.fromCharCode($0) + " " + $2 + " - " + $3 + " " + $4);}, currentChar, readingArray,theScope->currentKeyValueTrail, colonEncountered, justStartedArray);
	switch (currentChar) {
	case '{':
#ifdef DEBUG
#ifdef EMT
#else
		cout << "opening object\n";
#endif
#endif
		//EM_ASM_({console.log("__________________======================================__________OPENING object " + $0 + " " + String.fromCharCode($1));}, theState->stateNow, theState->reservedKey[0]);
		colonEncountered = false;
		//EM_ASM_({console.log("OPENING object " + $0);}, theState->stateNow);
		//EM_ASM_({console.log($0);}, (int)theState->stateNow);
#ifdef DEBUG
#ifdef EMT
#else
		cout << "reading states\n";
#endif
#endif
		if (isReadingDone()) {
			//readingDone();
			removeReadStates();
		}

		kvState = Key;
		readingArray = false;
		//if (justStartedArray == true) {

			//EM_ASM_({console.log("DONE adding new key value trail in array " + $0);}, currentKeyValueTrail);
		//}

#ifdef DEBUG
#ifdef EMT
#else
		cout << "checking states\n";
#endif
#endif
		if (theState->reservedKeySet && theState->stateNow == ArrayOpen) {
			input->currentReadKey[0] = '\0';
#ifdef DEBUG
#ifdef EMT
#else
			cout << "before strcat\n";
#endif
#endif
			strcat(input->currentReadKey, theState->reservedKey);
#ifdef DEBUG
#ifdef EMT
#else
			cout << "after strcat\n";
#endif
#endif
			if (strcmp(input->currentReadKey, "k") == 0) {
				//EM_ASM({console.log("                     k in readkey");});
			}
		}

		if (theState->stateNow == ArrayOpen) {
			addState(ScopeOpenInArray); //// ADD STATE
			//EM_ASM({console.log(" IN ARRAY ");});
		}
		else {
			addState(ScopeOpen); //// ADD STATE
		}

		checkScope();

		if (theState->stateNow == ScopeOpenInArray) {
			//EM_ASM({console.log("opening object in array");});

			prepareContainer(true);
			//EM_ASM({console.log("opening object in array 1");});
		}
		else {
			//if (! justStartedArray) {
				//if (theState->stateNow != ScopeOpenInArray) {
				//	checkScope();
				//}
				/*
				tempKeyValueTrail = newKeyValueTrail(theScope->currentKeyValueTrail);
				theScope->currentKeyValueTrail = tempKeyValueTrail;
				currentKeyValueTrail = theScope->currentKeyValueTrail;
				*/
				//EM_ASM_({console.log("DONE adding new key value trail " + $0);}, currentKeyValueTrail);
			//}

			prepareContainer(false);
		}


		struct KeyValueTrail* tempKeyValueTrail;
		//EM_ASM({console.log("new key value 1.0");});
		tempKeyValueTrail = newKeyValueTrail(theScope->currentKeyValueTrail);
		//EM_ASM({console.log("new key value 1.1");});
		theScope->currentKeyValueTrail = tempKeyValueTrail;
		//EM_ASM({console.log("new key value 1.2");});
		currentKeyValueTrail = theScope->currentKeyValueTrail;

		readingArray = false;
		justStartedArray = false;
		colonEncountered = false;

		//EM_ASM_({console.log($0);}, (int)theState->stateNow);
		//EM_ASM_({console.log("OPENED object " + $0);}, theState->stateNow);
		previousScopeClosure = false;
#ifdef DEBUG
#ifdef EMT
#else
		cout << "closing object\n";
#endif
#endif
		break;
	case '}':
		/*if (currentShapesItem != NULL && currentShapesItem->parent != NULL) {
			closureCount++;
		}*/
		justStartedArray = false;
		colonEncountered = false;

		//EM_ASM_({console.log("CLOSING object " + $0);}, theState->stateNow);
		if (isReadingDone()) {
			//readingDone();
			//EM_ASM_({console.log("CLOSING object reading " + $0);}, theState->stateNow);
			removeReadStates();
		}
		//EM_ASM_({console.log("CLOSING reading done " + $0);}, theState->stateNow);
		//EM_ASM_({console.log($0);}, (int)theState->stateNow);
		associateKeyValues();
		removeScope();
		removeState();
		currentKeyValueTrail = theScope->currentKeyValueTrail;

		//EM_ASM_({console.log("CLOSING removed scope " + $0);}, theState->stateNow);
		/*	readingArray = true;
			removeState();
			//EM_ASM_({console.log("<<CLOSING reverting to array " + $0);}, theState->stateNow);
		} else {
			if (lastStateBeforeReading() == ArrayOpen) {
				readingArray = true;
			} else {
				readingArray = false;
			}
			if (theScope != NULL) {
				currentKeyValueTrail = theScope->currentKeyValueTrail;
			} else {
				currentKeyValueTrail = NULL;
			}
		}
		*/
		//theScope->currentKeyValueTrail->keyValue->arrayValue = gotoParentArray(theScope->currentKeyValueTrail->keyValue);

	//EM_ASM_({console.log("___________________________________________________________________CLOSED object " + $0 + " " + String.fromCharCode($1));}, theState->stateNow, theState->reservedKey[0]);
		previousScopeClosure = true;
		break;
	case '[':
		//cout << "Opened array\n";
		if (colonEncountered) {
			justStartedArray = true;
		}
		else {
			justStartedArray = false;
		}
		//EM_ASM_({console.log("[OPENING array " + $0);}, theState->stateNow);
		if (isReadingDone()) {
			//readingDone();
			removeReadStates();
		}
		readingArray = true;
		kvState = Value;
		//if (theState->stateNow == ArrayOpen) {
		//}
		theScope->currentKeyValueTrail->keyValue = addChildArray(theScope->currentKeyValueTrail->keyValue);
		addState(ArrayOpen); //// ADD STATE

		previousScopeClosure = false;
		colonEncountered = false;

		if (strlen(input->currentReadKey) > 0) {
			theState->reservedKey[0] = '\0';
			strcat(theState->reservedKey, input->currentReadKey);
			theState->reservedKeySet = true;
		}
		else {
			if (theState->prev->reservedKeySet) {
				theState->reservedKey[0] = '\0';
				strcat(theState->reservedKey, theState->prev->reservedKey);
				theState->reservedKeySet = true;
			}
		}

		//EM_ASM_({console.log("[OPENED array " + $0);}, theState->stateNow);
		//cout << "Closed array\n";

		break;
	case ']':
		//EM_ASM_({console.log("[CLOSING array " + $0);}, theState->stateNow);
		//EM_ASM_({console.log($0);}, (int)theState->stateNow);
		if (!justStartedArray) {
			if (isReadingDone()) {
				//readingDone();
				//readingDone();
				removeReadStates();
			}
			if (currentShapesItem != NULL && currentShapesItem->parent != NULL && closureCount > 0) {
				//closureCount++;
				currentShapesItem = currentShapesItem->parent;
				//EM_ASM({console.log("UNWRAPPED array");});
				closureCount--;
			}
			//closedArray = true;
		}
		else {
			/*
			if (theScope->currentKeyValueTrail->keyValue->arrayValue->vector != NULL) {
				theScope->currentKeyValueTrail->keyValue->arrayValue->vector->prev->next = NULL;
				delete theScope->currentKeyValueTrail->keyValue->arrayValue->vector;
			}
			delete theScope->currentKeyValueTrail->keyValue->arrayValue;
			*/
			/*
			struct KeyValue* tempKeyValue;
			input->currentReadValue[0] = '\0';
			strcat(input->currentReadValue, "0");
			tempKeyValue = addKeyValue(theScope->currentKeyValueTrail->keyValue, input->currentReadKey, input->currentReadValue, false);
			theScope->currentKeyValueTrail->keyValue = tempKeyValue;
			*/
			removeReadStates();
		}
		//EM_ASM({console.log("[CLOSING reading completed");});
		kvState = Key;
		readingArray = false;
		//EM_ASM({console.log("[CLOSING reading states removed");});
		theScope->currentKeyValueTrail->keyValue->arrayValue = gotoParentArray(theScope->currentKeyValueTrail->keyValue);

		theState->reservedKeySet = false;
		theState->reservedKey[0] = '\0';

		removeState();
		if (theState->stateNow == ArrayOpen) {
			readingArray = true;
		}
		//EM_ASM({console.log("[CLOSING gone to parent array");});
		//EM_ASM_({console.log("[CLOSED array " + $0);}, theState->stateNow);
		previousScopeClosure = false;
		justStartedArray = false;
		/*if (currentShapesItem != NULL && currentShapesItem->parent != NULL) {
			currentShapesItem = currentShapesItem->parent;
		}*/
		break;
	case ':':
		colonEncountered = true;
		justStartedArray = false;
		if (isReadingDone()) {
		}
		removeReadStates();
		theState->keyEncountered = true;
		kvState = Value;
		previousScopeClosure = false;
		break;
	case '"':
		justStartedArray = false;
		colonEncountered = false;

		//EM_ASM({console.log("handling apostrophe 1");});
		if (quoteOpened && (theState->stateNow == KVReading || theState->stateNow == KVReadOpen)) {
			quoteOpened = false;
			removeReadStates();
		}
		else {
			quoteOpened = true;
			addState(KVReadOpen); //// ADD STATE
			//currentValue.clear();
			//input->currentValue[0] = '\0';
			resetCurrentValue();
		}

		previousScopeClosure = false;
		//EM_ASM({console.log("done handling apostrophe ");});
		break;
	case ',':
		justStartedArray = false;
		colonEncountered = false;
		//EM_ASM({console.log("handling comma ");});
		//addState(NewElement);
		if (isReadingDone()) {
			//readingDone();
			//readingDone();
			removeReadStates();
		}

		//EM_ASM_({console.log("handling comma 1 " + $0);}, theState->stateNow);
		//if (lastStateBeforeReading() == ArrayOpen || readingArray) {
		if (readingArray == true) {
			//EM_ASM({console.log("reading values into an array ");});
			kvState = Value;
		}
		else {
			kvState = Key;
		}

		theState->keyEncountered = false;
		//EM_ASM({console.log("done with comma ");});
		previousScopeClosure = false;
		break;
	default:
		//EM_ASM({console.log("DEFAULT 10.2 ");});
		if (currentChar != ' ' &&
			currentChar != '\f' &&
			currentChar != '\n' &&
			currentChar != '\r' &&
			currentChar != '\t' &&
			currentChar != '\v') {
			justStartedArray = false;
			colonEncountered = false;
			//if (theState->stateNow == KVReadOpen) {
			//EM_ASM({console.log("DEFAULT 10.1 ");});
			addState(KVReading); //// ADD STATE

		//EM_ASM({console.log("DEFAULT 10.2 ");});
		//}
		}
		else {
			if (theState->stateNow == KVReading || theState->stateNow == KVReadOpen) {
				//EM_ASM({console.log("DEFAULT 10.3 ");});
				addState(KVReading); //// ADD STATE
				//EM_ASM({console.log("DEFAULT 10.4 ");});
			}
		}
		//EM_ASM({console.log("DEFAULT 10.9 ");});
		previousScopeClosure = false;
	}

	if (theState->stateNow == KVReading) {
		//currentValue = currentValue + currentChar;
		//currentValue.append((char *)currentChar);
		//currentValue.append(1, currentChar);
		//strcat(input->currentValue, currentChar);
		appendCurrentValue(currentChar);
		//EM_ASM_({console.log(String.fromCharCode($0));}, currentChar);
	}

	//EM_ASM({console.log("DEFAULT 10.4 ");});
#ifdef DEBUG
#ifdef EMT
#else
	cout << "character processed\n";
#endif
#endif


	return 1;
}


int deserialize() {
	//struct ScopeTrail* theScope;
	//struct StateTrail* theState;
	theScope = new ScopeTrail;
	theState = new StateTrail;
	theScope->prev = NULL;
	theScope->next = NULL;
	theScope->scope = _noscope;
	theScope->start = theScope;
	struct KeyValueTrail* tempKeyValueTrail;
	tempKeyValueTrail = newKeyValueTrail(theScope->currentKeyValueTrail);
	theScope->currentKeyValueTrail = tempKeyValueTrail;
	currentKeyValueTrail = theScope->currentKeyValueTrail;


	theState->stateNow = Start;
	theState->start = theState;
	kvState = Key;
	input = new CurrentValues;
	//EM_ASM_({console.log("start state " + $0);}, theState);

		//EM_ASM({console.log('deserializing');});

	for (char& currentChar : jsonString) {

		checkCharacter(currentChar);

	}
	return 1;
}


int deserializeChar(char* theString, int theLength) {
	//struct ScopeTrail* theScope;
	//struct StateTrail* theState;
	theScope = new ScopeTrail;
	theState = new StateTrail;
	theScope->prev = NULL;
	theScope->next = NULL;
	theScope->scope = _noscope;
	theScope->start = theScope;
	struct KeyValueTrail* tempKeyValueTrail;
	tempKeyValueTrail = newKeyValueTrail(theScope->currentKeyValueTrail);
	theScope->currentKeyValueTrail = tempKeyValueTrail;
	currentKeyValueTrail = theScope->currentKeyValueTrail;


	theState->stateNow = Start;
	theState->start = theState;
	kvState = Key;
	input = new CurrentValues;
	//EM_ASM_({console.log("start state " + $0 + " " + $1);}, theState, theLength);


#ifdef DEBUG
#ifdef EMT
#else
	cout << "Deserializing begins here... " << theLength << "\n";
#endif
#endif
	char tempChar;
	for (int i = 0; i < theLength; i++) {
		tempChar = theString[i];
#ifdef DEBUG
#ifdef EMT
#else
		cout << tempChar << "\n";
#endif
#endif
		//EM_ASM_({console.log("deserializing" + String.fromCharCode($0));}, theString[i]);

		//std::strcat(tempChar, theString[i]);
		checkCharacter(tempChar);
	}
	return 1;
}