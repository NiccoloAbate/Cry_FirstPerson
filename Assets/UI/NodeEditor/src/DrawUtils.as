/**
 * ...
 * @author Niccolo Abate
 */
class DrawUtils
{
	
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
	
}