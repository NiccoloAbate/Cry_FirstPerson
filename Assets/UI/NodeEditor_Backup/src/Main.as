import DrawUtils;
/**
 * ...
 * @author Niccolo Abate
 */
class Main 
{
	
	var NodeEditors:Array = new Array;
	var ActiveNodeEditor:Number = -1;
	
	static var CRYENGINE:Boolean = true;
	static var CRYENGINE_SAVE:Boolean = true;
	
	static var FocusObjects:Array = new Array();
	static function HasFocus(Element:Object):Boolean
	{
		if (!Element)
		{
			if (FocusObjects.length > 0)
				return true;
			else
				return false;
		}
		for (var i:Number = 0; i < FocusObjects.length; i++)
		{
			if (Element == FocusObjects[i])
				return true;
		}
		return false;
	}
	static function GiveFocus(Element:Object):Void
	{
		FocusObjects.push(Element);
	}
	static function RemoveFocus(Element:Object):Void
	{
		if (HasFocus(Element))
		{
			for (var i:Number = 0; i < FocusObjects.length; i++)
			{
				if (Element == FocusObjects[i])
					FocusObjects.splice(i, 1);
			}
		}
	}
	static function ClearFocus():Void
	{
		FocusObjects = null;
		FocusObjects = new Array();
	}
	
	static var MAIN:Main;
	public static function main(swfRoot:MovieClip):Void 
	{
		//Stage.showMenu = false;
		Main.MAIN = new Main();
	}
	
	public function Main() 
	{
		
		var _Null_:MovieClip = _root.attachMovie("lib.DialogueNodeEditor_Top.png", "_Null_", _root.getNextHighestDepth());
		_Null_.removeMovieClip();
		
		var Curve:MovieClip = _root.createEmptyMovieClip("Curve", _root.getNextHighestDepth());
		Curve.lineStyle(5, 0xffffff, 100);
		Curve.moveTo(100,100);
		Curve.curveTo(300, 100, 300, 300);
		
		NewNodeEditor();
		var Editor:DialogueNodeEditor = GetActiveNodeEditor();
		
		Test();
		
	}
	
	public function GetActiveNodeEditor():DialogueNodeEditor
	{
		return NodeEditors[ActiveNodeEditor];
	}
	public function NewNodeEditor():Number
	{
		var Editor:DialogueNodeEditor = new DialogueNodeEditor(NodeEditors.length);
		
		ActiveNodeEditor = NodeEditors.push(Editor) - 1;
		return ActiveNodeEditor;
	}
	public function IsActiveEditor(Index:Number):Boolean
	{
		return (Index == ActiveNodeEditor);
	}
	
	public function Test():Void
	{
		
	}
	
}