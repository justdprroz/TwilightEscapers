# Generate directories

if (Test-Path -Path "build") {
} else {
    New-Item -Name "build" -ItemType "directory"
}

if (Test-Path -Path "tmp") {
} else {
    New-Item -Name "tmp" -ItemType "directory"
}

# Copy directories

Copy-Item -Path "assets" -Destination "build" -Recurse

# Go to directories

Set-Location -Path "tmp"

# Generate cmake files

cmake ..

Set-Location -Path ".."