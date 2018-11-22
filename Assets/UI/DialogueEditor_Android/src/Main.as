/**
 * ...
 * @author Niccolo Abate
 */
class Main 
{
	
	public static var MAIN:Main;
	public static function main(swfRoot:MovieClip):Void 
	{
		Main.MAIN = new Main();
	}
	
	public function Main() 
	{
		
		var Textfield:TextField = _root.createTextField("Test", _root.getNextHighestDepth(), 720 / 2, 1080 / 2, 720 / 2, 1080 / 2);
		Textfield.multiline = true;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		Textfield.type = "input";
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = 25;
		Textformat.color = 0x000000;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "center";
		Textformat.bold = false;
		Textfield.text = "This is my first test field object text.";
		Textfield.setTextFormat(Textformat);
		
		Textfield.bFirstChanged = false;
		
		Textfield.onSetFocus = function()
		{
			if (!Textfield.bFirstChanged)
			{
				Textfield.bFirstChanged = true;
				Textfield.text = "";
			}
			Textfield.setNewTextFormat(Textformat);
		}
		
		var TOPLEFT:MovieClip = DrawUtils.DrawRect(0, 0, 100, 100, 0x000000, "TOPLEFT");
		
	}
	
}