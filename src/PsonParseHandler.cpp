//
//
// PsonParseHandler.hpp
//
// Definition of the PsonParseHandler class based on Poco/JSON/ParseHandler extended with BinaryString.
//
// Copyright (c) 2020, Simplito Sp. z o.o.
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0

#include <Pson/PsonParseHandler.hpp>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/JSONException.h>


using Poco::Dynamic::Var;
using Poco::JSON::Object;
using Poco::JSON::Array;
using Poco::JSON::JSONException;

namespace Pson {

PsonParseHandler::PsonParseHandler(bool preserveObjectOrder) : Handler(),
	_preserveObjectOrder(preserveObjectOrder)
{
}


PsonParseHandler::~PsonParseHandler()
{
}


void PsonParseHandler::reset()
{
	while (!_stack.empty()) _stack.pop();
	_key = "";
	_result.empty();
	_resultFilled = false;
}


void PsonParseHandler::startObject()
{
	Object::Ptr newObj = new Object(_preserveObjectOrder);
	if (_stack.empty()) // The first object
	{
		if (_resultFilled) {
			throw JSONException("Result filled");
		}
		_result = newObj;
		_resultFilled = true;
	}
	else
	{
		Var parent = _stack.top();

		if (parent.type() == typeid(Array::Ptr))
		{
			Array::Ptr arr = parent.extract<Array::Ptr>();
			arr->add(newObj);
		}
		else if (parent.type() == typeid(Object::Ptr))
		{
			poco_assert_dbg(!_key.empty());
			Object::Ptr obj = parent.extract<Object::Ptr>();
			obj->set(_key, newObj);
			_key.clear();
		}
	}

	_stack.push(newObj);
}


void PsonParseHandler::endObject()
{
	if (!_stack.empty()) _stack.pop();
}


void PsonParseHandler::startArray()
{
	Array::Ptr newArr = new Array();

	if (_stack.empty()) // The first array
	{
		if (_resultFilled) {
			throw JSONException("Result filled");
		}
		_result = newArr;
		_resultFilled = true;
	}
	else
	{
		Var parent = _stack.top();

		if (parent.type() == typeid(Array::Ptr))
		{
			Array::Ptr arr = parent.extract<Array::Ptr>();
			arr->add(newArr);
		}
		else if (parent.type() == typeid(Object::Ptr))
		{
			poco_assert_dbg(!_key.empty());
			Object::Ptr obj = parent.extract<Object::Ptr>();
			obj->set(_key, newArr);
			_key.clear();
		}
	}

	_stack.push(newArr);
}


void PsonParseHandler::endArray()
{
	if (!_stack.empty()) _stack.pop();
}


void PsonParseHandler::key(const std::string& k)
{
	_key = k;
}


void PsonParseHandler::setValue(const Var& value)
{
	if (_stack.size())
	{
		Var parent = _stack.top();

		if (parent.type() == typeid(Array::Ptr))
		{
			Array::Ptr arr = parent.extract<Array::Ptr>();
			arr->add(value);
		}
		else if (parent.type() == typeid(Object::Ptr))
		{
			Object::Ptr obj = parent.extract<Object::Ptr>();
			obj->set(_key, value);
			_key.clear();
		}
	}
	else if (!_resultFilled) {
		_result = value;
		_resultFilled = true;
	}
	else
	{
		throw JSONException("Attempt to set value on an empty stack or filled result");
	}
}


} // namespace Pson
