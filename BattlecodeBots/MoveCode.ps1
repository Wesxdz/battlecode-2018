$currentDirectory = '' + (Get-Location)
$previousDirectory = $currentDirectory
Write-Host $currentDirectory
Write-Host $previousDirectory

Write-Host "Locating Battle Code Server Location"
Write-Host "	*Note: This process is done using the 'StartServer.ps1' file. Please ensure that file is in the Server Folder"

$destinationDirectory = ''
$configFile = $currentDirectory + "\MoveCode.ps1.config"

if(!(Test-Path $configFile)){
	
	$ps1File = $null;
	while(($ps1File -eq $null)){
	
		$previousDirectory = Split-Path $previousDirectory -Parent
		Write-Host $previousDirectory
		
		$ps1File = Get-Childitem –Path $previousDirectory -Include "StartServer.ps1" -File -Recurse | Select-Object -First 1

		if(!($?)){
			Write-Host "Could not find 'StartServer.ps1'. Please perform manually..."
			[void][System.Console]::ReadKey($true)
			Exit;
		}
	}
	
	$destinationDirectory = Split-Path $ps1File -Parent
	Out-File -FilePath $configFile -InputObject $destinationDirectory -Encoding ASCII -Width 50	
} else{
	$destinationDirectory = Get-Content $configFile
}

Write-Host ""
Write-Host "Current folder:" $currentDirectory
Write-Host "Destination folder:" $destinationDirectory
Write-Host "Copying data from Folder to Folder. This may take some time"

<#
$codeFiles = Get-Childitem –Path $currentDirectory -Include *.cpp, *.h, *.c, *.hpp -File -Recurse
foreach($file in $codeFiles){
	#Write-Host $file
	Copy-Item $file $destinationDirectory -Force -Verbose -Recurse
}

$currentDirectory += '\*.cpp'
$destinationDirectory += '\'
Write-Host ""
Write-Host "Current folder:" $currentDirectory
Write-Host "Destination folder:" $destinationDirectory
#Copy-Item -Path $currentDirectory -Destination $destinationDirectory -Force -Verbose
#>
$items = Get-ChildItem -Path $currentDirectory -Recurse -Include *.cpp, *.h, *.c, *.hpp, *.sh

foreach($item in $items){
	$toDirectory = Join-Path $destinationDirectory $item.FullName.Substring($currentDirectory.length)
	Write-Host $toDirectory
	New-Item -ItemType File -Path $toDirectory -Force
	Copy-Item -Path $item.FullName -Destination $toDirectory -Force  -Verbose
	#Write-Host (Join-Path $destinationDirectory $item.FullName.Substring($currentDirectory.length))
}
<#

Get-ChildItem -Path $currentDirectory -Recurse -Include "*.cpp, *.h, *.c, *.hpp" | 
    Copy-Item -Destination {
		Join-Path $destinationDirectory $_.FullName.Substring($currentDirectory.length)
    } -Force 
#>
#Copy-Item -Path $currentDirectory'.cpp', $currentDirectory'.hpp', $currentDirectory'.c', $currentDirectory'.h' -Destination $destinationDirectory -Force -Verbose
Write-Host "Succesful!"

#-Include *.cpp, *.h, *.c, *.hpp

#pause
[void][System.Console]::ReadKey($true)