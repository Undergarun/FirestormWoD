<?php
function CheckDistance($p_Obj1, $p_Obj2)
{
	$dx = $p_Obj1->{'x'} - $p_Obj2->{'x'};
	$dy = $p_Obj1->{'y'} - $p_Obj2->{'y'};
	$dz = $p_Obj1->{'z'} - $p_Obj2->{'z'};

	return sqrt($dx*$dx + $dy*$dy + $dz*$dz);
}

try
{
   $l_Connection = new PDO( 'mysql:host=127.0.0.1;dbname=world', 'user', 'pass');
}
catch (Exception $l_E)
{
   echo $l_E->getMessage();
   die();
}

$l_SpiritHealersSelect = $l_Connection->prepare("SELECT guid, position_x as x, position_y as y, position_z as z FROM creature WHERE id  = 6491");
$l_SpiritHealersSelect->execute();

$l_Result = $l_SpiritHealersSelect->fetchAll(PDO::FETCH_OBJ);


$DeleteArray = array();
echo "DELETE FROM creature WHERE id IN(";

foreach ($l_Result as $HealerPosition)
{
	if (in_array($HealerPosition->{'guid'}, $DeleteArray))
		continue;

	foreach ($l_Result as $HealerPositionCheck)
	{
		if ($HealerPosition->{'guid'} == $HealerPositionCheck->{'guid'})
			continue;

		if (CheckDistance($HealerPosition, $HealerPositionCheck) < 10)
		{
			array_push($DeleteArray, $HealerPositionCheck->{'guid'});
			echo $HealerPositionCheck->{'guid'} . ",";
		}
	}
}

echo ");";
?>