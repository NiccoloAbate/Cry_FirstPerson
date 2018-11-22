/**
 * ...
 * @author Niccolo Abate
 */
class DrawUtils
{
	
	static function DrawRect(X:Number, Y:Number, W:Number, H:Number, Col:Number, Name:String):MovieClip
	{
		var Rect:MovieClip = _root.createEmptyMovieClip(Name, _root.getNextHighestDepth());
		Rect._x = X;
		Rect._y = Y;
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
	
}