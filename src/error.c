#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine)
{
  //
  // Something horrible happened! You need to look
  // at file "pcFilename" at line "ulLine" to see
  // what error is being reported.
  //
  while(1)
  {
  }
}
#endif
