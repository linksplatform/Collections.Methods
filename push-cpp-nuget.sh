#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

sudo apt-get install xmlstarlet

NuSpecFile=$(echo cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/Platform.$REPOSITORY_NAME.TemplateLibrary.*.nuspec)
echo "$NuSpecFile"

cp "$NuSpecFile" "x.xml"

xmlstarlet el -u x.xml
xmlstarlet sel -t -m '/' -v . x.xml
Version=$(xmlstarlet sel -t -m '/' -v . x.xml)
echo "$Version"

#Version=$(xmlstarlet sel -t -m '//VersionPrefix[1]' -v . -n <"Platform.$REPOSITORY_NAME/Platform.$REPOSITORY_NAME.csproj")

# Get version string
#PackageFileNamePrefix="Platform.$REPOSITORY_NAME/bin/Release/Platform.$REPOSITORY_NAME."
#PackageFileNameSuffix=".nupkg"
#PackageFileName=$(echo "$PackageFileNamePrefix"*"$PackageFileNameSuffix")
#Version="${PackageFileName#$PackageFileNamePrefix}"
#Version="${Version%$PackageFileNameSuffix}"

#OldVersion=$(xmlstarlet sel -t -m '//version[1]' -v . -n <"cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/Platform.$REPOSITORY_NAME.TemplateLibrary.*.nuspec")
#xmlstarlet edit --inplace --update '/preferences/options/dbUser' --value 123 preferences.xml

# Pack NuGet package
# dotnet pack -c Release
cp "cpp/Platform.$REPOSITORY_NAME/*.h" "cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/lib/native/include/"
cp "cpp/Platform.$REPOSITORY_NAME/*.cpp" "cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/lib/native/include/"
echo "Files copied."

nuget pack "$NuSpecFile"

# Ensure NuGet package does not exist
NuGetPackageUrl="https://globalcdn.nuget.org/packages/Platform.$REPOSITORY_NAME.TemplateLibrary.$Version$PackageFileNameSuffix"
NuGetPackageUrl=$(echo "$NuGetPackageUrl" | tr '[:upper:]' '[:lower:]')
NuGetPageStatus="$(curl -Is "$NuGetPackageUrl" | head -1)"
StatusContents=( $NuGetPageStatus )
if [ "${StatusContents[1]}" == "200" ]; then
  echo "NuGet with current version is already pushed."
  exit 0
fi

# Push NuGet package
# dotnet nuget push ./**/*.nupkg -s https://api.nuget.org/v3/index.json -k "${NUGETTOKEN}" 
nuget push ./**/*.nupkg -s https://api.nuget.org/v3/index.json -k "${NUGETTOKEN}" 

# Clean up
find . -type f -name '*.nupkg' -delete