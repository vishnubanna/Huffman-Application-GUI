//
//  ViewController.swift
//  Huffman
//
//  Created by Vishnu Banna on 12/26/19.
//  Copyright © 2019 Vishnu Banna. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {


  @IBOutlet weak var inFile: NSTextField!
  @IBOutlet weak var outFile: NSTextField!
  @IBOutlet weak var ShowText: NSScrollView!
  
  var inp : UnsafeMutablePointer<Int8>? = nil
  var lenfile : Int = 0
  
  
  override func viewDidLoad() {
    super.viewDidLoad()

    // Do any additional setup after loading the view.
  }

  override var representedObject: Any? {
    didSet {
    // Update the view, if already loaded.
    }
  }
  
  @IBAction func CompBut(_ sender: Any){
    let compressed : ([UInt8], String?) = compress()
    //let pointer = UnsafeBufferPointer(start: compressed.0, count: compressed.0.count)
    let data = Data(buffer:UnsafeBufferPointer(start: compressed.0, count: compressed.0.count))
    let text : NSTextView = ShowText.documentView! as! NSTextView
    
    if (compressed.1 != nil){
      text.string = compressed.1!
      //let path = outFile.stringValue
      let url1 = FindFile()
      
     if (url1 != nil){
          let url = url1?.appendingPathComponent(outFile.stringValue, isDirectory: false)
          print(url!.path)
          
          do{
            try data.write(to: url!)
          }
          catch {
            print(error)
          }
        }
      }
  }
  
  @IBAction func DeCompBut(_ sender: Any){
   let compressed : ([UInt8], String?) = decompress()
    //let pointer = UnsafeBufferPointer(start: compressed.0, count: compressed.0.count)
    let data = Data(buffer:UnsafeBufferPointer(start: compressed.0, count: compressed.0.count))
    let text : NSTextView = ShowText.documentView! as! NSTextView
    
    if (compressed.1 != nil){
      text.string = compressed.1!
      //let path = outFile.stringValue
      let url1 = FindFile()
    if (url1 != nil){
        let url = url1?.appendingPathComponent(outFile.stringValue, isDirectory: false)
        print(url!.path)
        
        do{
          try data.write(to: url!)
        }
        catch {
          print(error)
        }
      }
    }
  }
  
  @IBAction func FinderIN(_ sender: Any) {
    let filesel = FindFile()
    if (filesel != nil){
      inFile.stringValue = "\(filesel!)"
    }
    else{
      inFile.stringValue = "NO FILE SELECTED"
    }
    let output: (UnsafeMutablePointer<Int8>?, Int) = open(filename: filesel)
    let text : NSTextView = ShowText.documentView! as! NSTextView
    text.string = String(bytesNoCopy: output.0!, length: Int(100), encoding: String.Encoding.ascii, freeWhenDone: false)!
    inp = output.0!
    lenfile = output.1;
  }
  
  func FindFile() -> URL?{
    let start = NSOpenPanel()
    
    start.canChooseFiles = true
    start.canChooseDirectories = true
    start.allowsMultipleSelection = false
    
    if (start.runModal() == NSApplication.ModalResponse.OK){
      let URLreq = start.url
      
      return URLreq
    }
    else{
      return nil
    }
  }
  
  func open(filename:URL?) -> (UnsafeMutablePointer<Int8>?, Int){
    if (filename == nil){
      return (nil, 0);
    }
    let io = try? FileHandle(forReadingFrom: filename!)
    let data = io?.readDataToEndOfFile()
    let valuint : [UInt8] = Array(data!)
    var valhold : [Int8] = []
    for val in valuint{
      valhold.append(Int8(bitPattern: val))
    }
    let values  = UnsafeMutablePointer<Int8>.allocate(capacity: valhold.count)
    print(valhold)
    values.initialize(from: valhold, count: valhold.count)
    //print("\(values)")
    io?.closeFile()
    return (values, valhold.count)
    
  }
  
  func compress() -> ([UInt8], String?){
    let textholder : NSTextView = ShowText.documentView! as! NSTextView
    print(textholder.string)
    //let input: UnsafeMutablePointer? = UnsafeMutablePointer(mutating: (textholder.string as NSString).utf8String)
    
    let output = write1(inp, Int32(lenfile))
    let size = writelen(inp, Int32(lenfile))
    print(size)
    print(type(of: output))
    
    let finbuff = UnsafeMutableRawBufferPointer(start: UnsafeMutableRawPointer(output), count: Int(size))
    let finarr : [UInt8] = Array(finbuff)
    //print(finarr)
    let final = String(bytesNoCopy: output!, length: Int(size), encoding: String.Encoding.ascii, freeWhenDone: true)
    
    return (finarr, final)
  }
  
  func decompress() -> ([UInt8], String?){
    let textholder : NSTextView = ShowText.documentView! as! NSTextView
    print(textholder.string)
    //let input: UnsafeMutablePointer? = UnsafeMutablePointer(mutating: (textholder.string as NSString).utf8String)
    print(inp as Any, lenfile)
    let output = read1(inp, Int32(lenfile))
    print(inp as Any, lenfile)
    let size = readlen(inp, Int32(lenfile))
    print(size)
    print(type(of: output))
    
    let finbuff = UnsafeMutableRawBufferPointer(start: UnsafeMutableRawPointer(output), count: Int(size))
    let finarr : [UInt8] = Array(finbuff)
    //print(finarr)
    let final = String(cString: output!)
    print("swift val \(final) \(size)")
    
    return (finarr, final)
  }

}
