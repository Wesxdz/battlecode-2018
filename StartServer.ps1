<#
$containerName = "BattleCodeServer"
$containerAbsPath = "C:\Users\Flameo326\Documents\IDEs\BattleCodeServer\"

docker run -it --privileged -p 16147:16147 -p 6147:6147 --name $containerName  -v "%~dp0:/player" "battlecode/battlecode-2018"

if(!$?){
	docker run -it --privileged -p 16147:16147 -p 6147:6147 --name $containerName  -v "${containerAbsPath}:/player" "battlecode/battlecode-2018"
}

$containerID = (docker ps -aqf "name=$containerName")
Write-Host "This Containers ID is $containerID"

#pause
[void][System.Console]::ReadKey($true)
#>

$currentDirectory = '' + (Get-Location)
$buildPath = $currentDirectory + ":/player"

docker pull battlecode/battlecode-2018

Write-Host ""
Write-Host "Deleting all containers"
docker stop (docker ps -a -q)
docker rm $(docker ps -a -q)

Write-Host ""
Write-Host "Starting up the server"
docker run -it --privileged -p 16147:16147 -p 6147:6147 --name "BattleCodeServer" -v $buildPath "battlecode/battlecode-2018"

if(!$?){
	docker run -it --privileged -p 16147:16147 -p 6147:6147 --name "BattleCodeServer" -v "C:\Users\Flameo326\Documents\IDEs\BattleCodeServer\:/player" "battlecode/battlecode-2018"
}

#pause
[void][System.Console]::ReadKey($true)

