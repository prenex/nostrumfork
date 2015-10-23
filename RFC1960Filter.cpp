/*
 * nOSGi Project - http://www-vs.informatik.uni-ulm.de/proj/nosgi/
 *
 * Copyright (C) 2009-2011 Aspectix Research Team,
 * Institute of Distributed Systems, Ulm University, Germany
 *
 * This file is part of the nOSGi Project, which is released
 * under the terms of the GNU General Public License.
 * See the LICENSE.txt file in the root directory of the software
 * distribution for more details.
 */

//This implementation based on RFC1960Filter.java
//of OSGi platform Concierge
#include <nosgi/framework/RFC1960Filter.h>

#include <stack>
#include <nosgi/framework/InvalidSyntaxException.h>
#include <nosgi/framework/ServiceReference.h>
//import org.osgi.framework.InvalidSyntaxException;

/*	private static final Filter NULL_FILTER = new Filter() {
		public boolean match(final ServiceReference reference) {
			return true;
		}

		public boolean match(final Dictionary dictionary) {
			return true;
		}
	};*/

string substrTrim(const string &str, int start, int end) {
	while (str.at(start)==' ' && start<=end) {
		start++;
	}
	while (str.at(end)==' ' && start>=end) {
		end--;
	}
	return str.substr(start, end-start);
}

RFC1960Filter::RFC1960Filter(int op) 
	: operat(op) {
}

Filter *RFC1960Filter::fromString(const string &filterString)
			/*throws InvalidSyntaxException*/ {

	/*if (filterString == "") {
		return NULL_FILTER;
	}*/

	stack<RFC1960Filter*> myStack;

	/*try {*/

		const int len = filterString.length();

		int last = -1;
		int oper = 0;
		string id;
		int comparator = -1;

		const char *chars = filterString.c_str();
		//myStack.clear();

		for (int i = 0; i < len; i++) {
			switch (chars[i]) {
			case '(':
			{	// lookahead ...
				char nextChar = chars[i + 1];
				while (nextChar == ' ' || nextChar == '\t'/*Character.isWhitespace(nextChar)*/) {
					i++;
					nextChar = chars[i + 1];
				}
				if (nextChar == '&') {
					myStack.push(new RFC1960Filter(AND_OPERATOR));
					continue;
				} else if (nextChar == '|') {
					myStack.push(new RFC1960Filter(OR_OPERATOR));
					continue;
				} else if (nextChar == '!') {
					myStack.push(new RFC1960Filter(NOT_OPERATOR));
					continue;
				} else {
					if (last == -1) {
						last = i;
					} else {
						throw InvalidSyntaxException(
							"Surplus left paranthesis at: "
										/*+ filterString.substr(i),
								filterString*/);
					}
				}
			}
				continue;
			case ')':
				if (last == -1) {
					RFC1960Filter *filter = myStack.top();
					myStack.pop();
					if (myStack.empty()) {
						return filter;
					} else {
						delete filter;
					}
					RFC1960Filter *parent = myStack.top();
					if (parent->operat == NOT_OPERATOR
							&& !parent->operands.empty()) {
						throw InvalidSyntaxException(
								"Unexpected literal: "/*
										+ filterString.substr(i),
								filterString*/);
					}
					parent->operands.push_back(filter);//
					if (i == len - 1) {
						throw InvalidSyntaxException(
								"Missing right paranthesis at the end."/*,
								filterString*/);
					}
				} else {
					if (oper == 0) {
						throw InvalidSyntaxException(
								"Missing operator."/*, filterString*/);
					}
					if (myStack.empty()) {
						if (i == len - 1) {

							// just a single simple filter
							string value = filterString.substr(++oper,
									len - 1);
							if (value == "*" && comparator == EQUALS) {
								comparator = PRESENT;
								//value = NULL;
								value = "";
							}

							return new RFC1960SimpleFilter(id, comparator,
									value);
						} else {
							throw InvalidSyntaxException(
									"Unexpected literal: "
											/*+ filterString.substring(i),
									filterString*/);
						}
					}

					// get the parent from stack
					RFC1960Filter *parent = myStack.top();

					string value = filterString.substr(++oper, i);
					if (value == "*" && comparator == EQUALS) {
						comparator = PRESENT;
						//value = NULL;
					}
					// link current element to parent
					parent->operands.push_back(new RFC1960SimpleFilter(id,
							comparator, value));//

					oper = 0;
					last = -1;
					id = ""; //NULL
					comparator = -1;
				}
				continue;
			case '~':
				if (oper == 0 && chars[i + 1] == '=') {

					id = substrTrim(filterString, last + 1, i);
					comparator = APPROX;
					oper = ++i;
					continue;
				} else {
					throw InvalidSyntaxException(
							"Unexpected character " /*+ chars[i + 1],
							filterString*/);
				}
			case '>':
				if (oper == 0 && chars[i + 1] == '=') {
					id = substrTrim(filterString, last + 1, i);
					comparator = GREATER;
					oper = ++i;
					continue;
				} else {
					throw InvalidSyntaxException(
							"Unexpected character " /*+ chars[i + 1],
							filterString*/);
				}
			case '<':
				if (oper == 0 && chars[i + 1] == '=') {
					id = substrTrim(filterString, last + 1, i);
					comparator = LESS;
					oper = ++i;
					continue;
				} else {
					throw InvalidSyntaxException(
							"Unexpected character " /*+ chars[i + 1],
							filterString*/);
				}
			case '=':
				// could also be a "=*" present production.
				// if this is the case, it is fixed later, because
				// value=* and value=*key would require a lookahead of at
				// least two. (the symbol "=*" alone is ambigous).
				id = substrTrim(filterString, last + 1, i);
				comparator = EQUALS;
				oper = i;
				continue;
			}
		}
		Filter *filter = myStack.top();
		myStack.pop();
		return filter;
	/*} catch (EmptyStackException e) {
		throw new InvalidSyntaxException(
				"Filter expression not well-formed.", filterString);
	}*/
}


bool RFC1960Filter::match(const ServiceReference &reference) {
	/*try {*/
		return match(reference.properties);
	/*} catch (Exception e) {
		// so this was not instance of ServiceReferenceImpl. Someone
		// must have created an own implementation.
		final Dictionary dict = new Hashtable();
		String[] keys = reference.getPropertyKeys();
		for (int i = 0; i < keys.length; i++) {
			dict.put(keys[i], reference.getProperty(keys[i]));
		}
		return match(dict);
	}*/
}

bool RFC1960Filter::match(const map<string,Property*> *values) {
	if (operat == AND_OPERATOR) {
		const unsigned int opersize = operands.size();
		for (unsigned int i = 0; i < opersize; i++) {
			if (!operands[i]->match(values)) {
				return false;
			}
		}
		return true;
	} else if (operat == OR_OPERATOR) {
		const unsigned int opersize = operands.size();
		for (unsigned int i = 0; i < opersize; i++) {
			if (operands[i]->match(values)) {
				return true;
			}
		}
		return false;
	} else if (operat == NOT_OPERATOR) {
		return !(operands.at(0)->match(values));
	}
	throw InvalidSyntaxException("PARSER ERROR");
}
/*
	public String toString() {
		if (operator == NOT_OPERATOR) {
			return "(!" + operands.get(0) + ")";
		}
		final StringBuffer buffer = new StringBuffer(
				operator == AND_OPERATOR ? "(&" : "(|");
		Filter[] operandArray = (Filter[]) operands.toArray(new Filter[operands
				.size()]);
		for (int i = 0; i < operandArray.length; i++) {
			buffer.append(operandArray[i]);
		}
		buffer.append(")");
		return buffer.toString();
	}

	public boolean equals(final Object obj) {
		if (obj instanceof RFC1960Filter) {
			RFC1960Filter filter = (RFC1960Filter) obj;

			if (operands.size() != filter.operands.size()) {
				return false;
			}
			final Filter[] operandArray = (Filter[]) operands
					.toArray(new Filter[operands.size()]);
			final Filter[] operandArray2 = (Filter[]) filter.operands
					.toArray(new Filter[operands.size()]);
			for (int i = 0; i < operandArray.length; i++) {
				if (!operandArray[i].equals(operandArray2[i])) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	public int hashCode() {
		return toString().hashCode();
	}
*/


RFC1960Filter::RFC1960SimpleFilter::RFC1960SimpleFilter(const string &i, const int comp,
		const string &v)
	: id(i), comparator(comp), value(v) {
}

bool RFC1960Filter::RFC1960SimpleFilter::match(const map<string,Property*> *m) {
	// just by chance, try if the case sensitive matching returns a
	// result.
	map<string,Property*>::const_iterator cit(m->find(id));
	if (cit == m->end()) {
		// no ? Then try lower case.
		string s;
		transform(id.begin(), id.end(), s.begin(), ::tolower);
		cit = m->find(s);
/*
		if (cit == m->end()) {
			// bad luck, try case insensitive matching of all keys
			for (Enumeration keys = map.keys(); keys.hasMoreElements();) {
				String key = (String) keys.nextElement();
				if (key.equalsIgnoreCase(id)) {
					temp = map.get(key);
					break;
				}
			}
		}
*/

		if (cit == m->end()) {
			return false;
		}
	}

	// are we just checking for presence ? Then we are done ...
	if (comparator == PRESENT) {
		return true;
	}

/*
	try {*/
	switch(cit->second->getType()) {
	case Property::STRING:
		return compareString(value, comparator,
				cit->second->getString());
	case Property::INT:
		switch (comparator) {
		case GREATER: //atoi ist schneller als streams
			return cit->second->getInt() >= atoi(value.c_str());
		case LESS:
			return cit->second->getInt() <= atoi(value.c_str());
		default:
			return cit->second->getInt() == atoi(value.c_str());
		}
	case Property::LONG:
		switch (comparator) {
		case GREATER:
			return cit->second->getLong() >= atol(value.c_str());
		case LESS:
			return cit->second->getLong() <= atol(value.c_str());
		default:
			return cit->second->getLong() == atol(value.c_str());
		}
        case Property::SHORT:
		switch (comparator) {
		case GREATER:
			return cit->second->getShort() >= atoi(value.c_str());
		case LESS:
			return cit->second->getShort() <= atoi(value.c_str());
		default:
			return cit->second->getShort() == atoi(value.c_str());
		}
	case Property::DOUBLE:
		switch (comparator) {
		case GREATER:
			return cit->second->getDouble() >= atof(value.c_str());
		case LESS:
			return cit->second->getDouble() <= atof(value.c_str());
		default:
			return cit->second->getDouble() == atof(value.c_str());
		}
	case Property::FLOAT:
		switch (comparator) {
		case GREATER:
			return cit->second->getFloat() >= atof(value.c_str());
		case LESS:
			return cit->second->getFloat() <= atof(value.c_str());
		default:
			return cit->second->getFloat() == atof(value.c_str());
		}
/*	case Property::BYTE:
	{
		try {
			return compareTyped(Byte.decode(value), comparator,
					(Byte) attr);
		} catch (Throwable t) {
		}
	}
	// all other are less frequent and are handled as
	// Comparables.
	return compareReflective(value, comparator, (Comparable) attr);
*/
			// all the numbers checkings run a lot faster when compared
			// in a primitive typed way
		/*	return compareNumber(value, comparator, (Number) attr);
		}*/ /*else if (attr instanceof String[]) {
			final String[] array = (String[]) attr;
			if (array.length == 0) {
				return false;
			}
			final String val = comparator == APPROX ? stripWhitespaces(value)
					: value;
			for (int i = 0; i < array.length; i++) {
				if (compareString(val, comparator, array[i])) {
					return true;
				}
			}
			return false;
		} else if (attr->getType() == Property::BOOL) {
			return ((comparator == EQUALS || comparator == APPROX) && ((Boolean) attr)
					.equals(Boolean.valueOf(value)));
		} else if (attr->getType() == Property::CHAR) {
			return value.length() == 1 ? compareTyped(new Character(
					value.charAt(0)), comparator, (Character) attr)
					: false;
		} else if (attr instanceof Vector) {
			Vector vec = (Vector) attr;
			final Object[] obj = new Object[vec.size()];
			vec.copyInto(obj);
			return compareArray(value, comparator, obj);
		} else if (attr instanceof Object[]) {
			return compareArray(value, comparator, (Object[]) attr);
		} else if (attr.getClass().isArray()) {
			for (int i = 0; i < Array.getLength(attr); i++) {
				final Object obj = Array.get(attr, i);
				if (obj instanceof Number
						&& compareNumber(value, comparator,
								(Number) obj)
						|| (obj instanceof Comparable && compareReflective(
								value, comparator, (Comparable) obj))) {
					return true;
				}
			}
			return false;
		} else if (attr instanceof Comparable) {
			return compareReflective(value, comparator,
					(Comparable) attr);
		}*/
	}
	return false;
	/*} catch (Throwable t) {
		return false;
	}*/
}

bool RFC1960Filter::RFC1960SimpleFilter::match(const ServiceReference &reference) {
	//try {
		return match(reference.properties);
	/*} catch (Exception e) {
		// so this was not instance of ServiceReferenceImpl. Someone
		// must
		// have created an own implementation.
		final Dictionary dict = new Hashtable();
		String[] keys = reference.getPropertyKeys();
		for (int i = 0; i < keys.length; i++) {
			dict.put(keys[i], reference.getProperty(keys[i]));
		}
		return match(dict);
	}*/
}
/*
private: */
bool RFC1960Filter::RFC1960SimpleFilter::compareString(const string &val,
		const int comparator, const string *attr) {
	const string value;/* = comparator == APPROX
				? stripWhitespaces(val).toLowerCase() : val;*/
	const string attribute;/* = comparator == APPROX
				? stripWhitespaces(attr).toLowerCase() : attr;*/
	switch (comparator) {
		case APPROX:
		case EQUALS:
			return stringCompare(value, 0, attribute, 0) == 0;
		case GREATER:
			return stringCompare(value, 0, attribute, 0) <= 0;
		case LESS:
			return stringCompare(value, 0, attribute, 0) >= 0;
		default:
			cerr << "Found illegal comparator" << endl;
			return false;
			/*throw IllegalStateException("Found illegal comparator.");*/
	}
}
/*
		private static boolean compareNumber(final String value,
				final int comparator, final Number attr) {
		}

		private static boolean compareTyped(final Object typedVal,
				final int comparator, final Comparable attr) {
			switch (comparator) {
			case EQUALS:
			case APPROX:
				return attr.equals(typedVal);
			case GREATER:
				return attr.compareTo(typedVal) >= 0;
			case LESS:
				return attr.compareTo(typedVal) <= 0;
			default:
				throw new IllegalStateException("Found illegal comparator.");
			}
		}

		private static boolean compareArray(final String value,
				final int comparator, final Object[] array) {
			for (int i = 0; i < array.length; i++) {
				final Object obj = array[i];
				if (obj instanceof String) {
					if (compareString(value, comparator, (String) obj)) {
						return true;
					}
				} else if (obj instanceof Number) {
					if (compareNumber(value, comparator, (Number) obj)) {
						return true;
					}
				} else if (obj instanceof Comparable) {
					if (compareReflective(value, comparator, (Comparable) obj)) {
						return true;
					}
				}
			}
			return false;
		}

		private static boolean compareReflective(final String val,
				final int comparator, final Comparable attr) {
			final Class clazz = attr.getClass();
			Object typedVal = null;
			try {
				final Constructor constr = clazz.getConstructor(STRINGCLASS);
				typedVal = constr.newInstance(new Object[] { val });
				return compareTyped(typedVal, comparator, attr);
			} catch (Exception didNotWork) {
				return false;
			}
		}

		private static String stripWhitespaces(final String s) {
			return s.replace(' ', '\0');
		}
*/
int RFC1960Filter::RFC1960SimpleFilter::stringCompare(const string &s1,
		int p1,	const string &s2, int p2) {
	const int l1 = s1.length();
	if (p1 == l1) {
		return 0;
	}

	char *c1 = (char *)s1.c_str();
	char *c2 = (char *)s2.c_str();
	const int l2 = s2.length();

	while (p1 < l1 && p2 < l2) {
		if (c1[p1] == c2[p2]) {
			p1++;
			p2++;
			continue;
		}
		if (c1[p1] > 'A' && c1[p1] < 'Z') {
			c1[p1] = (char) (c1[p1] + 32);
		}
		if (c2[p2] > 'A' && c2[p2] < 'Z') {
			c2[p2] = (char) (c2[p2] + 32);
		}
		if (c1[p1] == c2[p2]) {
			p1++;
			p2++;
			continue;
		}
		if (c1[p1] == '*') {
			p1++;
			do {
				if (stringCompare(c1, p1, c2, p2) == 0) {
					return 0;
				}
				p2++;
			} while (l2 - p2 > -1);
			return 1;
		} else {
			if (c1[p1] < c2[p2]) {
				return -1;
			} else if (c1[p1] > c2[p2]) {
				return 1;
			}
		}
	}
	if (p1 == l1 && p2 == l2 && c1[p1 - 1] == c2[p2 - 1]) {
		return 0;
	}
	if (c1[p1 - 1] == '*' && p1 == l1 && p2 == l2) {
		return 0;
	}
	const int min = l1 < l2 ? l1 : l2;
	return l1 == min ? -1 : 1;
}
/*
		public String toString() {
			return "(" + id + OP[comparator] + (value == null ? "" : value)
					+ ")";
		}

		public boolean equals(final Object obj) {
			if (obj instanceof RFC1960SimpleFilter) {
				final RFC1960SimpleFilter filter = (RFC1960SimpleFilter) obj;
				return (comparator == filter.comparator)
						&& id.equals(filter.id) && (value.equals(filter.value));
			}
			return false;
		}

		public int hashCode() {
			return toString().hashCode();
		}*/
