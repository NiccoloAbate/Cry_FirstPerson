/**
 * ...
 * @author Niccolo Abate
 */
class Main 
{
	
	static var MAIN:Main;
	public var inputBox:InputBox;
	public static function main(swfRoot:MovieClip):Void 
	{
		MAIN = new Main();
	}
	
	public function Main() 
	{
		inputBox = new InputBox();
		
		_root.ClearResponses = function():Void
		{
			Main.MAIN.inputBox.ClearResponses();
		}
		_root.SetResponseNum = function(Num:Number):Void
		{
			Main.MAIN.inputBox.SetResponseNum(Num);
		}
		_root.SetResponse = function(Index:Number, Text:String):Void
		{
			Main.MAIN.inputBox.SetResponse(Index, Text);
		}
		
	}
	
}