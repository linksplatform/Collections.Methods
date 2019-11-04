#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

sudo apt install nuget

# Get version string
PackageSpecFileNamePrefix="cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/Platform.$REPOSITORY_NAME.TemplateLibrary."
PackageSpecFileNameSuffix=".nuspec"
PackageSpecFileName=$(echo "$PackageSpecFileNamePrefix"*"$PackageSpecFileNameSuffix")
Version="${PackageSpecFileName#$PackageSpecFileNamePrefix}"
Version="${Version%$PackageSpecFileNameSuffix}"

# Ensure NuGet package does not exist
NuGetPackageUrl="https://globalcdn.nuget.org/packages/Platform.$REPOSITORY_NAME.TemplateLibrary.$Version.nupkg"
NuGetPackageUrl=$(echo "$NuGetPackageUrl" | tr '[:upper:]' '[:lower:]')
NuGetPageStatus="$(curl -Is "$NuGetPackageUrl" | head -1)"
StatusContents=( $NuGetPageStatus )
if [ "${StatusContents[1]}" == "200" ]; then
  echo "NuGet with current version is already pushed."
  exit 0
fi

#NuSpecFile=$(echo cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/Platform.$REPOSITORY_NAME.TemplateLibrary.*.nuspec)
#echo "$NuSpecFile"

#cp "$NuSpecFile" "x.xml"
#
#xmlstarlet el -u x.xml
#xmlstarlet sel -t -m '/package' -v . x.xml
#Version=$(xmlstarlet sel -t -m '/package' -v . x.xml)
#echo "$Version"

#Version=$(xmlstarlet sel -t -m '//VersionPrefix[1]' -v . -n <"Platform.$REPOSITORY_NAME/Platform.$REPOSITORY_NAME.csproj")



#OldVersion=$(xmlstarlet sel -t -m '//version[1]' -v . -n <"cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/Platform.$REPOSITORY_NAME.TemplateLibrary.*.nuspec")
#xmlstarlet edit --inplace --update '/preferences/options/dbUser' --value 123 preferences.xml

# Pack NuGet package
# dotnet pack -c Release

LibNativeIncludeDirectory="cpp/Platform.$REPOSITORY_NAME/NuGetPackageSource/lib/native/include"

mkdir -p "$LibNativeIncludeDirectory"

cp "cpp/Platform.$REPOSITORY_NAME/"*.h "$LibNativeIncludeDirectory/"
cp "cpp/Platform.$REPOSITORY_NAME/"*.cpp "$LibNativeIncludeDirectory/"
echo "Files copied."

nuget pack "$PackageSpecFileName"

# Push NuGet package
# dotnet nuget push ./**/*.nupkg -s https://api.nuget.org/v3/index.json -k "${NUGETTOKEN}" 
nuget push ./**/*.nupkg -Source https://api.nuget.org/v3/index.json -ApiKey "${NUGETTOKEN}" 

# Clean up
find . -type f -name '*.nupkg' -delete