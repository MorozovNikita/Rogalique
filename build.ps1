param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",
    [ValidateSet("x64", "Win32")]
    [string]$Platform = "x64",
    [switch]$Tests
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    throw "vswhere.exe not found."
}

$msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
if (-not $msbuild) {
    throw "MSBuild.exe not found."
}

function Invoke-Project([string]$Project) {
    # Два слэша в конце: PowerShell иначе съедает кавычку у пути с пробелами.
    $solutionDir = $root.TrimEnd('\') + '\\'
    & $msbuild $Project "/p:Configuration=$Configuration" "/p:Platform=$Platform" "/p:SolutionDir=$solutionDir" "/m:1" "/v:minimal"
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

Invoke-Project (Join-Path $root "Rogalique.vcxproj")

if ($Tests) {
    Invoke-Project (Join-Path $root "EngineTest\EngineTest.vcxproj")
    $testExe = Join-Path $root "$Platform\$Configuration\EngineTest.exe"
    if (-not (Test-Path $testExe)) {
        $testExe = Join-Path $root "EngineTest\$Platform\$Configuration\EngineTest.exe"
    }
    & $testExe
    exit $LASTEXITCODE
}
