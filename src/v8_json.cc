
using namespace v8;

Local<Value> json_parse_or_raw (std::string const& text) {
  HandleScope scope;
  Handle<Context> context = Context::GetCurrent();

  Local<Value> v8text = v8::String::New(text.c_str(), text.size());

  Handle<Object> global = context->Global();
  Handle<Value> jsonValue = global->Get(v8::String::New("JSON"));

  if (! jsonValue->IsObject()) {
    return scope.Close(v8text);
  }

  Handle<Object> json = jsonValue->ToObject();
  Handle<Value> parse = json->Get(v8::String::New("parse"));

  if (parse.IsEmpty() || ! parse->IsFunction()) {
    return scope.Close(v8text);
  }

  // cast into a function
  Handle<Function> parseFunction = Handle<Function>::Cast(parse);
  Local<Value> result;

  TryCatch trycatch;
  result = parseFunction->Call(json, 1, &v8text);
  if (result.IsEmpty()) {
    result = v8text;
  }

  return scope.Close(result);
}
