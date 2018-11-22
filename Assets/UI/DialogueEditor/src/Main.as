import Nodes.DialogueInfoNode;
/**
 * ...
 * @author Niccolo Abate
 */
class Main 
{
	
	var NodeEditors:Array = new Array;
	var ActiveNodeEditor:Number = -1;
	
	var DialogueFileNames:Array;
	
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
		
		NewNodeEditor();
		var Editor:DialogueNodeEditor = GetActiveNodeEditor();
		
		Test();
		
		_root.SetVisible = function(Visible:Boolean):Void
		{
			Main.MAIN.SetNodeEditorVisible(Visible);
		}
		_root.SetDialogueFileNames = function(FileNames:Array):Void
		{
			Main.MAIN.SetDialogueFileNames(FileNames);
		}
		_root.AddDialogueFileName = function(FileName:String):Void
		{
			Main.MAIN.AddDialogueFileName(FileName);
		}
		
		_root.OnRightClick = function():Void
		{
			Main.MAIN.OnRightClick();
		}
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
	
	// Cryengine Funtions
	public function SetNodeEditorVisible(Visible:Boolean):Void
	{
		GetActiveNodeEditor().Background._visible = Visible;
	}
	public function SetDialogueFileNames(FileNames:Array):Void
	{
		DialogueFileNames = FileNames;
	}
	public function AddDialogueFileName(FileName:String):Void
	{
		if (!DialogueFileNames)
			DialogueFileNames = new Array();
		DialogueFileNames.push(FileName);
	}
	
	public function OnRightClick():Void
	{
		GetActiveNodeEditor().OnRightClick();
	}
	//~Cryengine Functions
	
	static public function TranslationBase(MC:MovieClip, NewParentSpace:MovieClip):Object
	{
		var Base:Object = new Object();
		Base.X = NewParentSpace._xmouse - MC._xmouse;
		Base.Y = NewParentSpace._ymouse - MC._ymouse;
		return Base;
	}
	static public function TranslatePos(MC:MovieClip, X:Number, Y:Number, NewParentSpace:MovieClip):Object
	{
		var Pos:Object = new Object();
		var BaseX:Number = NewParentSpace._xmouse - MC._xmouse;
		var BaseY:Number = NewParentSpace._ymouse - MC._ymouse;
		Pos.X = BaseX + X;
		Pos.Y = BaseY + Y;
		return Pos;
	}
	static public function TranslateMCPos(MC:MovieClip, NewParentSpace:MovieClip):Object
	{
		var Pos:Object = new Object();
		var BaseX:Number = NewParentSpace._xmouse - MC._parent._xmouse;
		var BaseY:Number = NewParentSpace._ymouse - MC._parent._ymouse;
		Pos.X = BaseX + MC._x;
		Pos.Y = BaseY + MC._y;
		return Pos;
	}
	
}