/**
 * ...
 * @author Niccolo Abate
 */
[Embed fonts.hemi_head.tt (Bold Italic 95)]
class Main 
{
	
	static var gMain:Main;
	static var UsableScreen_X = 1920;
	static var UsableScreen_Y = 1080;
	static var EndL:String = "\n"
	
	var SpeechBubbles:Array = new Array(0);
	
	public static function main(swfRoot:MovieClip):Void 
	{
		Main.gMain = new Main();
	}
	
	public function Main() 
	{
		var i:Number = NewBubble(200, 200, 1);
		SetBubbleText(i, "Asswhole" + EndL + "high my name is tom" + EndL + "my favorite neybor is charlie's horse, gerrit" + EndL + "the jones family is a fun time" + EndL + "there churn are my best buds" + EndL + "Bleeeeuhhu" + EndL + "hump my hump grill" + EndL + "plz" + EndL + "rz");
		
		_root.NewBubble = function(x:Number, y:Number, size:Number):Number
		{
			return Main.gMain.NewBubble(x, y, size);
		}
		_root.UpdateBubble = function(Index:Number, x:Number, y:Number, size:Number):Void
		{
			Main.gMain.UpdateBubble(Index, x, y, size);
		}
		_root.SetBubbleText = function(Index:Number, Text:String):Void
		{
			Main.gMain.SetBubbleText(Index, Text);
		}
		_root.DeleteBubble = function(Index:Number):Void
		{
			Main.gMain.DeleteBubble(Index);
		}
	}
	
	public function NewBubble(x:Number, y:Number, size:Number):Number
	{
		var NewBubble:SpeechBubble = new SpeechBubble(x, y, size);
		SpeechBubbles.push(NewBubble);
		return SpeechBubbles.length - 1;
	}
	public function UpdateBubble(Index:Number, x:Number, y:Number, size:Number):Void
	{
		var Bubble:SpeechBubble = SpeechBubbles[Index];
		Bubble.Update(x, y, size);
	}
	public function SetBubbleText(Index:Number, Text:String):Void
	{
		var Bubble:SpeechBubble = SpeechBubbles[Index];
		Bubble.ChangeText(Text);
	}
	public function DeleteBubble(Index:Number):Void
	{
		var Bubble:SpeechBubble = SpeechBubbles[Index];
		Bubble.Destroy();
		SpeechBubbles[Index] = null;
	}
	
}