/**
 * ...
 * @author Niccolo Abate
 */
class DrawUtils
{
	static private var Background_Col:Number = 0x555555;
	static private var Background_BorderSize:Number = 2;
	static private var Background_BorderCol:Number = 0x333333;
	
	static private var TextSize:Number = 12;
	static private var TextCol:Number = 0xffffff;
	static private var AltTextCol:Number = 0xcccccc;
	
	
	static function DrawRect(Name:String, X:Number, Y:Number, W:Number, H:Number, Col:Number, Thickness:Number, BorderCol:Number, Parent:MovieClip):MovieClip
	{
		var Rect:MovieClip;
		if (Parent)
			Rect = Parent.createEmptyMovieClip(Name, Parent.getNextHighestDepth());
		else
			Rect = _root.createEmptyMovieClip(Name, _root.getNextHighestDepth());
		
		Rect._x = X;
		Rect._y = Y;
		if (Thickness && BorderCol)
			Rect.lineStyle(Thickness, BorderCol, 100, true, "none", "round", "miter", 1);
		else
			Rect.lineStyle(0, Col, 100, true, "none", "round", "miter", 1);
		Rect.beginFill(Col);
		Rect.moveTo(0, 0);
		Rect.lineTo(W, 0);
		Rect.lineTo(W, H);
		Rect.lineTo(0, H);
		Rect.lineTo(0, 0);
		Rect.endFill();
		return Rect;
	}
	static function AddRect(MC:MovieClip, X:Number, Y:Number, W:Number, H:Number, Col:Number, Thickness:Number, BorderCol:Number):Void
	{
		MC._x = X;
		MC._y = Y;
		if (Thickness && BorderCol)
			MC.lineStyle(Thickness, BorderCol, 100, true, "none", "round", "miter", 1);
		else
			MC.lineStyle(0, Col, 100, true, "none", "round", "miter", 1);
		MC.beginFill(Col);
		MC.moveTo(0, 0);
		MC.lineTo(W, 0);
		MC.lineTo(W, H);
		MC.lineTo(0, H);
		MC.lineTo(0, 0);
		MC.endFill();
	}
	static function ReDrawRect(MC:MovieClip, X:Number, Y:Number, W:Number, H:Number, Col:Number, Thickness:Number, BorderCol:Number):Void
	{
		MC.clear();
		MC._x = X;
		MC._y = Y;
		if (Thickness && BorderCol)
			MC.lineStyle(Thickness, BorderCol, 100, true, "none", "round", "miter", 1);
		else
			MC.lineStyle(0, Col, 100, true, "none", "round", "miter", 1);
		MC.beginFill(Col);
		MC.moveTo(0, 0);
		MC.lineTo(W, 0);
		MC.lineTo(W, H);
		MC.lineTo(0, H);
		MC.lineTo(0, 0);
		MC.endFill();
	}
	static function MakeRectInfo(Name:String, X:Number, Y:Number, W:Number, H:Number, Col:Number, Thickness:Number, BorderCol:Number, Parent:MovieClip):Object
	{
		var Info:Object = new Object;
		Info.Name = Name;
		Info.X = X;
		Info.Y = Y;
		Info.W = W;
		Info.H = H;
		Info.Col = Col;
		Info.Thickness = Thickness;
		Info.BorderCol = BorderCol;
		Info.Parent = Parent;
		return Info;
	}
	static function DrawRect_FromInfo(Info:Object):MovieClip
	{
		if (!Info)
			return null;
		return DrawRect(Info.Name, Info.X, Info.Y, Info.W, Info.H, Info.Col, Info.Thickness, Info.BorderCol, Info.Parent);
	}
	
	static public function GiveInputTextField(name:String, x:Number, y:Number, w:Number, h:Number, DefaultText:String, Parent:MovieClip):MovieClip
	{
		if (!Parent)
			Parent = _root;
		
		var Background:MovieClip = DrawUtils.DrawRect(Parent._name + "_TextInput_" + name + "_Background", x, y + 15, w, h - 15, Background_Col + 0x444444, Background_BorderSize, Background_BorderCol + 0x444444, Parent);
		
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = TextSize;
		Textformat.color = AltTextCol;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "left";
		Textformat.bold = false;
		
		var NameField:TextField = Background.createTextField(Parent._name + "_TextInput_" + name + "_NameField", Background.getNextHighestDepth(), 0, -15, w, 15);
		NameField.multiline = false;
		NameField.wordWrap = false;
		NameField.embedFonts = true;
		NameField.setNewTextFormat(Textformat);
		NameField.text = name;
		NameField.setTextFormat(Textformat);
		
		Textformat.color = TextCol;
		
		var Textfield:TextField = Background.createTextField(Parent._name + "_TextInput_" + name + "_Textfield", Background.getNextHighestDepth(), 0, 0, w, h - 15);
		Textfield.multiline = false;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		Textfield.type = "input";
		Textfield.setNewTextFormat(Textformat);
		Textfield.text = DefaultText;
		Textfield.setTextFormat(Textformat);
		
		Background.Type = "TextInput";
		Background.Namefield = NameField;
		Background.Textfield = Textfield;
		
		return Background;
	}
}