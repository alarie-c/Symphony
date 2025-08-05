param (
    [Alias("t")]
    [switch]$Test
)

$CurrentDirectoryName = Split-Path -Leaf (Get-Location)
if ($CurrentDirectoryName -eq "build") {
    Set-Location ..
}

if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

$BuildDir = "build"
Set-Location $BuildDir

cmake .. 
if ($LASTEXITCODE -ne 0) {
    Write-Error "cmake failed"
    Set-Location ..
    exit 1
}

$Target = "build/symphc"
if ($Test) {
    make tests
    if ($LASTEXITCODE -ne 0) {
        Write-Error "make target `tests` failed"
        Set-Location ..
        exit 1
    }
    $Target = "build/tests"
} else {
    make symphc
    if ($LASTEXITCODE -ne 0) {
        Write-Error "make target `symphc` failed"
        Set-Location ..
        exit 1
    }
}

# Clear the terminal, get out of the build folder, and then run the executable
Clear-Host
Set-Location ..
Start-Process -NoNewWindow -Wait $Target
