/**
 * ...
 * @author Niccolo Abate
 */
class Dialogue
{
	
	public var Name:String;
	public var Nodes:Array = new Array;
	
	public function Destroy():Void
	{
		
	}
	
	public function Save():Void
	{
		if (Main.CRYENGINE_SAVE)
		{
			fscommand("Save_Start", Name);
			
			for (var i:Number = 0; i < Nodes.length; i++)
			{
				Nodes[i].Save();
			}
			
		}
	}
	
	public function DeleteNode(Node:DialogueNode):Void
	{
		SpliceElement(Nodes, Node);
		Node.Destroy();
	}
	
	static private function SpliceElement(Arr:Array, Element:Object):Void
	{
		for (var i:Number = 0; i < Arr.length; i++)
		{
			if (Element == Arr[i])
			{
				Arr.splice(i, 1);
				return;
			}
		}
	}
}