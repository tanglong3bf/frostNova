import request from '@/utils/request'

export interface LoginResponse {
  token: string;
}

export const login = (username: string, password: string) => {
  return request.post<Data<LoginResponse>, LoginResponse, LoginParams>(
    '/login',
    {
      username,
      password
    })
}