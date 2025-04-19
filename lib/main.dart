import 'dart:ffi' as ffi;
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:ffi/ffi.dart';

typedef AnalyzeFunc = ffi.Pointer<ffi.Int8> Function(ffi.Pointer<ffi.Int8>);
typedef Analyze = ffi.Pointer<ffi.Int8> Function(ffi.Pointer<ffi.Int8>);

late final Analyze analyzeCode;

void main() {
  final dylib = Platform.isAndroid
      ? ffi.DynamicLibrary.open("liblexer.so")
      : ffi.DynamicLibrary.process();

  analyzeCode = dylib
      .lookup<ffi.NativeFunction<AnalyzeFunc>>("analyze_code")
      .asFunction();

  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(home: LexicalPage());
  }
}

class LexicalPage extends StatefulWidget {
  @override
  _LexicalPageState createState() => _LexicalPageState();
}

class _LexicalPageState extends State<LexicalPage> {
  final TextEditingController _controller = TextEditingController();
  String result = "";

  void analyze() {
    final input = _controller.text;
    final ptr = input.toNativeUtf8();
    final outputPtr = analyzeCode(ptr.cast());
    setState(() {
      result = outputPtr.cast<Utf8>().toDartString();
    });
    calloc.free(ptr);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("C Lexical Analyzer")),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            TextField(controller: _controller, maxLines: 6),
            ElevatedButton(onPressed: analyze, child: Text("Analyze")),
            SizedBox(height: 20),
            Text(result),
          ],
        ),
      ),
    );
  }
}
